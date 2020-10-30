cmake_minimum_required(VERSION 3.15.7)
project(calcParser-tests)

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
            set_property(TARGET ${TEST_PREFIX_NAME} PROPERTY
                    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
            if (TEST_PREFIX_LINKS)
                target_link_libraries(${TEST_PREFIX_NAME} ${TEST_PREFIX_LINKS})
            endif (TEST_PREFIX_LINKS)
            target_include_directories(${TEST_PREFIX_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/Catch2/single_include)
            add_test(NAME ${TEST_PREFIX_NAME} COMMAND ${TEST_PREFIX_NAME} ${TEST_PREFIX_ARGS})
        else (TEST_PREFIX_FILES)
            message(SEND_ERROR "No files specified for ${TEST_PREFIX_NAME}")
        endif (TEST_PREFIX_FILES)
    else (TEST_PREFIX_NAME)
        message(SEND_ERROR "No name specified for test!")
    endif (TEST_PREFIX_NAME)
endfunction()

# Add tests here!
# define_test(NAME NAME_HERE FILES FILES_HERE LINKS OPTIONAL_LINKS_HERE)

