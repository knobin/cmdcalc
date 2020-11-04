cmake_minimum_required(VERSION 3.12.4)
project(CalcEval-tests)

set(CALC_INCLUDE "${CMAKE_SOURCE_DIR}/include")
set(CALC_SRC "${CMAKE_SOURCE_DIR}/src")

# Function for define tests.
function(define_test)
    cmake_parse_arguments(
        TEST_PREFIX
        ""
        "NAME"
        "ARGS;FILES;LINKS"
        ${ARGN}
    )

    if (TEST_PREFIX_NAME)
        if (TEST_PREFIX_FILES)
            add_executable(${TEST_PREFIX_NAME} ${TEST_PREFIX_FILES})
            if (TEST_PREFIX_LINKS)
                target_link_libraries(${TEST_PREFIX_NAME} ${TEST_PREFIX_LINKS} Catch2::Catch2WithMain)
            else()
                target_link_libraries(${TEST_PREFIX_NAME} Catch2::Catch2WithMain)
            endif (TEST_PREFIX_LINKS)
            target_include_directories(${TEST_PREFIX_NAME} PRIVATE ${CALC_INCLUDE})
            add_test(NAME ${TEST_PREFIX_NAME} COMMAND ${TEST_PREFIX_NAME} ${TEST_PREFIX_ARGS})
        else (TEST_PREFIX_FILES)
            message(SEND_ERROR "No files specified for ${TEST_PREFIX_NAME}")
        endif (TEST_PREFIX_FILES)
    else (TEST_PREFIX_NAME)
        message(SEND_ERROR "No name specified for test!")
    endif (TEST_PREFIX_NAME)
endfunction()

# Add tests here!
define_test(NAME TokenTest FILES TokenTests.cpp LINKS CalcEval)
define_test(NAME ScannerTest FILES ScannerTests.cpp LINKS CalcEval)
define_test(NAME ParserTest FILES ParserTests.cpp LINKS CalcEval)

