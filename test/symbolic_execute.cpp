#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "coreir.h"
#include "coreir/passes/transform/rungenerators.h"
#include "coreir/simulator/interpreter.h"
#include "coreir/libs/commonlib.h"
#include "coreir/libs/rtlil.h"
#include "coreir/libs/commonlib.h"

using namespace std;
using namespace CoreIR;
using namespace CoreIR::Passes;

namespace UnSynth {

  TEST_CASE("Unsynthesize fadd") {

    Context* c = newContext();

    CoreIRLoadLibrary_rtlil(c);

    if (!loadFromFile(c,"./adder.json")) {
      std::cout << "Could not Load from json!!" << std::endl;
      c->die();
    }

    c->runPasses({"rungenerators", "flatten", "split-inouts","delete-unused-inouts",
          "deletedeadinstances","add-dummy-inputs", "packconnections",
          "removeconstduplicates","cullzexts", "clockifyinterface"});

    //c->runPasses({"rungenerators", "flattentypes", "flatten", "deletedeadinstances", "packconnections", "removeconstduplicates"});

    Namespace* g = c->getNamespace("global");
    for (auto ms : g->getModules()) {
      cout << ms.first << endl;
    }
    
    Module* adder = g->getModule("adder");

    if (!saveToFile(g, "adder_preprocessed.json", adder)) {
      cout << "Could not save to json!!" << endl;
      c->die();
    }
    
    
    cout << "Adder fields" << endl;
    for (auto field : adder->getType()->getFields()) {
      cout << "\t" << field << endl;
    }

    SimulatorState sim(adder);
    sim.setValue("self.rst", BitVector(1, 1));
    sim.setClock("self.clk", 0, 1);
    sim.setValue("self.input_a", BitVector(32, 0));
    sim.setValue("self.input_b", BitVector(32, 0));            
    sim.setValue("self.input_a_stb", BitVector(1, 0));
    sim.setValue("self.input_b_stb", BitVector(1, 0));            
    sim.setValue("self.output_z_ack", BitVector(1, 0));            
    sim.execute();

    cout << "a_ack = " << sim.getBitVec("self.input_a_ack") << endl;    
    cout << "z_stb = " << sim.getBitVec("self.output_z_stb") << endl;
    
    sim.setValue("self.rst", BitVector(1, 0));
    sim.setClock("self.clk", 0, 1);
    sim.setValue("self.input_a", BitVector(32, 1));
    sim.setValue("self.input_a_stb", BitVector(1, 1));
    sim.execute();

    cout << "a_ack = " << sim.getBitVec("self.input_a_ack") << endl;    
    cout << "z_stb = " << sim.getBitVec("self.output_z_stb") << endl;
    
    sim.setClock("self.clk", 0, 1);
    sim.setValue("self.input_a", BitVector(32, 1));
    sim.setValue("self.input_a_stb", BitVector(1, 1));
    sim.execute();

    cout << "a_ack = " << sim.getBitVec("self.input_a_ack") << endl;    
    cout << "z_stb = " << sim.getBitVec("self.output_z_stb") << endl;
    
    sim.setClock("self.clk", 0, 1);
    sim.setValue("self.input_a", BitVector(32, 1));
    sim.setValue("self.input_a_stb", BitVector(1, 1));
    sim.execute();

    cout << "a_ack = " << sim.getBitVec("self.input_a_ack") << endl;    
    cout << "z_stb = " << sim.getBitVec("self.output_z_stb") << endl;

    REQUIRE(true);
  }

}
