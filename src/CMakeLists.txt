cmake_minimum_required(VERSION 3.15.7)
project(CalcEval)

# Source files
set(SOURCE_FILES Error.cpp Parser.cpp Scanner.cpp Token.cpp)

# Define library
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Compiler flags
if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /permissive- /W4 /w14640)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC
            -Wall -Wextra -Wpedantic -fvisibility=hidden -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align
            -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wdouble-promotion -Wformat=2)
endif ()
