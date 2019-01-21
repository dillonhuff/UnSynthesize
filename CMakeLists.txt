cmake_minimum_required(VERSION 2.6)

project(UNSYNTHESIZE)

SET(EXTRA_CXX_COMPILE_FLAGS "-std=c++11 -I./src -I./test -I./benchmarks -I/opt/local/include -I/usr/include -I/usr/local/include/ -I/usr/lib -O2 -Wall -fPIC -I./z3-4.8.4.d6df51951f4c-x64-ubuntu-16.04/include -L./z3-4.8.4.d6df51951f4c-x64-ubuntu-16.04/bin -fno-rtti")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(./src/)

SET(CPP_FILES ./src/symbolic_execute.cpp)

add_library(unsynthesize ${CPP_FILES})
target_link_libraries(unsynthesize z3)

# Test executables
SET(ALL_TEST_FILES ./test/symbolic_execute.cpp)

add_executable(all-tests ${ALL_TEST_FILES})

target_link_libraries(all-tests unsynthesize)
target_link_libraries(all-tests coreir coreir-rtlil coreir-commonlib)
