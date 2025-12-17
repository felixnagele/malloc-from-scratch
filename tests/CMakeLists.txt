# CMakeLists.txt for tests/

# Automatically find all .cpp files with the name test_ in tests/ (including subdirectories)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp")
message(STATUS "Found tests: ${TEST_SOURCES}")

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)

    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name}
        PRIVATE
            malloc-from-scratch
    )

    target_include_directories(${test_name}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Add strace tests if strace is available
find_program(STRACE_EXECUTABLE strace)
if(STRACE_EXECUTABLE)
    message(STATUS "strace found: ${STRACE_EXECUTABLE}")
    message(STATUS "Adding strace-based memory leak tests")

    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)

        add_test(
            NAME ${test_name}_strace
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../tools/strace_test.sh
                    $<TARGET_FILE:${test_name}>
        )

        set_tests_properties(${test_name}_strace PROPERTIES
            LABELS "strace;memory"
        )
    endforeach()
else()
    message(WARNING "strace not found. Memory leak detection tests will be skipped.")
    message(WARNING "Install strace: sudo apt install strace (Ubuntu/Debian)")
endif()

