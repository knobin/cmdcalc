cmake_minimum_required(VERSION 3.15.7)
project(cmdCalc VERSION 1 DESCRIPTION "Command-line calculator in C++.")

set(CMAKE_CXX_STANDARD 17)

# General options
option(BUILD_TESTS "Build test programs" OFF)

# Build Type
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DPARSER_DEBUG)
    set(PARSER_BUILD_TYPE "Debug")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(PARSER_BUILD_TYPE "Release")
else()
    message(STATUS "No build type specified. Default is Release")
    set(PARSER_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${PARSER_BUILD_TYPE}")

# Library
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# App
add_subdirectory(${CMAKE_SOURCE_DIR}/app)

# Tests
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif (BUILD_TESTS)

# Print options
message(STATUS "Build Tests: ${BUILD_TESTS}")
