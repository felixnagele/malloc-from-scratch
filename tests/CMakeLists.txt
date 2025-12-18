# CMakeLists.txt for tests/

# Automatically find all .cpp files with test_ prefix (including subdirectories)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp")
message(STATUS "Found tests: ${TEST_SOURCES}")

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    file(RELATIVE_PATH test_relative_path ${CMAKE_CURRENT_SOURCE_DIR} ${test_file})
    get_filename_component(test_category ${test_relative_path} DIRECTORY)

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

    # Set test category label based on subdirectory
    if(test_category)
        set_tests_properties(${test_name} PROPERTIES
            LABELS "${test_category}"
        )
    endif()
endforeach()

# Add strace tests for all categories except those that would produce false positives
# Default: strace enabled for ALL tests
# Excluded only for specific categories
find_program(STRACE_EXECUTABLE strace)
if(STRACE_EXECUTABLE)
    message(STATUS "strace found: ${STRACE_EXECUTABLE}")
    message(STATUS "Adding strace-based memory leak tests (excluding: unit, edge_cases, invalid)")

    # Categories to EXCLUDE from strace tests
    set(STRACE_EXCLUDE_CATEGORIES "unit" "edge_cases" "invalid")

    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        file(RELATIVE_PATH test_relative_path ${CMAKE_CURRENT_SOURCE_DIR} ${test_file})
        get_filename_component(test_category ${test_relative_path} DIRECTORY)

        # Default: add strace unless category is in exclude list
        set(ADD_STRACE TRUE)
        foreach(category ${STRACE_EXCLUDE_CATEGORIES})
            if(test_category STREQUAL ${category})
                set(ADD_STRACE FALSE)
                break()
            endif()
        endforeach()

        if(ADD_STRACE)
            add_test(
                NAME ${test_name}_strace
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../tools/strace_test.sh
                        $<TARGET_FILE:${test_name}>
            )

            set_tests_properties(${test_name}_strace PROPERTIES
                LABELS "strace;memory;${test_category}"
            )
        endif()
    endforeach()
else()
    message(WARNING "strace not found. Memory leak detection tests will be skipped.")
    message(WARNING "Install strace: sudo apt install strace (Ubuntu/Debian)")
endif()
