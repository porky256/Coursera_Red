cmake_minimum_required(VERSION 3.15)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)
add_executable(WebServer main.cpp test_runner.h http_request.h stats.h stats.cpp)