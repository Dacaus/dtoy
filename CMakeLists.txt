cmake_minimum_required(VERSION 3.31)
project(dtoy VERSION 1.0.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)


add_subdirectory(libs)
add_subdirectory(main)
add_subdirectory(tests)
