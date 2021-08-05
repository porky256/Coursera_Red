cmake_minimum_required(VERSION 3.15)
project(TextEditor)

set(CMAKE_CXX_STANDARD 14)
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O2")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

add_executable(TextEditor main.cpp test_runner.h)