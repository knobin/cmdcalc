cmake_minimum_required(VERSION 3.12.4)
project(CalcEval)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Header files
set(HEADER_FILES ${INCLUDE_DIR}/calceval/Error.hpp
    ${INCLUDE_DIR}/calceval/Parser.hpp
    ${INCLUDE_DIR}/calceval/ParserLogic.hpp
    ${INCLUDE_DIR}/calceval/ParserLogic.tpp
    ${INCLUDE_DIR}/calceval/Scanner.hpp 
    ${INCLUDE_DIR}/calceval/Token.hpp
    ${INCLUDE_DIR}/calceval/type/Base.hpp
    ${INCLUDE_DIR}/calceval/type/Double.hpp)

# Source files
set(SOURCE_FILES ${SOURCE_DIR}/Error.cpp 
    ${SOURCE_DIR}/Scanner.cpp 
    ${SOURCE_DIR}/Token.cpp)

# Define library
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}")

# Compiler flags
if (MSVC)
    string(REGEX REPLACE "/W[3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    target_compile_options(${PROJECT_NAME} PUBLIC /permissive- /w14640)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC
            -Wall -Wextra -Wpedantic -fvisibility=hidden -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align
            -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wdouble-promotion -Wformat=2)
endif ()
