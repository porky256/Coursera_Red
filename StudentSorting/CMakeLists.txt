cmake_minimum_required(VERSION 3.15)
project(StudentSorting)

set(CMAKE_CXX_STANDARD 20)

add_executable(StudentSorting main.cpp test_runner.h student.h profile.h)