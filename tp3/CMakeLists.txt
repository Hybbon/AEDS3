# CMakeLists.txt
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TP3 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -pg")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TP3_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TP3_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TP3_BINARY_DIR}/bin")

add_subdirectory(${TP3_SOURCE_DIR}/src)
