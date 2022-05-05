##################################################################
#
# Unit Test build recipe
#
# This CMake file contains the recipe for building cFS app unit tests.
# It is invoked from the parent directory when unit tests are enabled.
#
# Alan S. Gibson, GSFC-587
#
##################################################################


# Use the UT assert public API, and allow direct
# inclusion of source files that are normally private
# ${osal_MISSION_DIR}/ut_assert/inc is location of ut_assert headers
include_directories(${osal_MISSION_DIR}/ut_assert/inc)
# ${osal_MISSION_DIR}/src/os/inc has some headers we need for testing
include_directories(${osal_MISSION_DIR}/src/os/inc)
# ${CMAKE_CURRENT_SOURCE_DIR}/inc is location of app test headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
# ${CMAKE_CURRENT_SOURCE_DIR}/utilities is location of app test utilities
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utilities)
# ${CMAKE_CURRENT_SOURCE_DIR}/stubs is location of app source stubs
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stubs)
# ${PROJECT_SOURCE_DIR}/fsw/src is location of app source files
include_directories(${PROJECT_SOURCE_DIR}/fsw/src)
#${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/inc is location of osal coverage stub headers
include_directories(${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/inc)
include_directories(${cfs_lib_MISSION_DIR}/fsw/public_inc)
include_directories(${cfs_lib_MISSION_DIR}/fsw/src)
#include_directories(${fs_lib_MISSION_DIR}/fsw/public_inc)
#include_directories(${fs_lib_MISSION_DIR}/fsw/src)

message(STATUS "project source dir: ${PROJECT_SOURCE_DIR}")

set(app_stub_files)
set(app_stub_and_utility_filenames)
set(app_ut_utility_files)
set(app_ut_utility_filenames)
aux_source_directory("${PROJECT_SOURCE_DIR}/unit-test/utilities" app_ut_utility_files)
foreach(SOURCE_FILE ${app_ut_utility_files})
 list(APPEND app_stub_and_utility_filenames "${SOURCE_FILE}")
endforeach()
list(REVERSE app_stub_and_utility_filenames)
aux_source_directory("${PROJECT_SOURCE_DIR}/unit-test/stubs" app_stub_files)
foreach(SOURCE_FILE ${app_stub_files})
 list(APPEND app_stub_and_utility_filenames "${SOURCE_FILE}")
endforeach()
list(APPEND app_stub_and_utility_filenames "${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/src/libc-stdio-stubs.c")
message(STATUS app_stub_and_utility_filenames)
# add both utility and stubs to the coverage stubs
# TODO: currently adding both is considered a work-around
# utilities should be added separately when a method for it is devised
add_cfe_coverage_stubs("fm" ${app_stub_and_utility_filenames})

set(source_files_under_test)
aux_source_directory("${PROJECT_SOURCE_DIR}/fsw/src" source_files_under_test)

#foreach(SRCFILE "${PROJECT_SOURCE_DIR}/fsw/src/fm_app.c")
foreach(SRCFILE ${source_files_under_test})
    # Get the base sourcefile name as a module name without path or the  
    # extension, this will be used as the base name of the unit test file.
    get_filename_component(MODULE_NAME "${SRCFILE}" NAME_WE)

    # Use the module name to make the tests' name by adding _tests to the end
    set(TESTS_NAME "${MODULE_NAME}_tests")
    
    # Make the test sourcefile name with unit test path and extension
    set(TESTS_SOURCE_FILE "${PROJECT_SOURCE_DIR}/unit-test/${TESTS_NAME}.c")

    add_cfe_coverage_test("fm" "${TESTS_NAME}" "${TESTS_SOURCE_FILE}" "${SRCFILE}")

    target_link_libraries("coverage-fm-${TESTS_NAME}-testrunner" ut_libc_stubs)
    add_cfe_coverage_dependency("fm" "${TESTS_NAME}" "fm")
    add_cfe_coverage_dependency("fm" "${TESTS_NAME}" "cfs_lib")
#    add_cfe_coverage_dependency("fm" "${TESTS_NAME}" "fs_lib")
   
endforeach()

