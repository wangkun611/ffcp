cmake_minimum_required(VERSION 3.22)

project(ffcp)
enable_testing()

add_executable(ffcp main.c)

add_subdirectory(tests)
