cmake_minimum_required(VERSION 3.12.4)
project(cmdCalc VERSION 1 DESCRIPTION "Command-line calculator in C++.")

set(CMAKE_CXX_STANDARD 17)

# General options
option(BUILD_TESTS "Build test programs" ON)

# Library
add_subdirectory(${CMAKE_SOURCE_DIR}/calceval)

# App
add_subdirectory(${CMAKE_SOURCE_DIR}/app)

# Tests
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/Catch2)
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif (BUILD_TESTS)

# Print options
message(STATUS "Build Tests: ${BUILD_TESTS}")
