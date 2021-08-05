cmake_minimum_required(VERSION 3.15)
project(ConcurrentMap)

set(CMAKE_CXX_STANDARD 20)

add_executable(ConcurrentMap main.cpp test_runner.h profile.h)