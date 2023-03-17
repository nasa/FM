##################################################################
#
# Unit Test build recipe
#
# This CMake file contains the recipe for building cFS app unit tests.
# It is invoked from the parent directory when unit tests are enabled.
#
##################################################################

add_cfe_coverage_stubs("fm_internal"
  utilities/fm_test_utils.c
  stubs/fm_global_stubs.c
  stubs/fm_cmds_stubs.c
  stubs/fm_cmd_utils_stubs.c
  stubs/fm_cmd_utils_handlers.c
  stubs/fm_compression_stubs.c
  stubs/fm_dispatch_stubs.c
  stubs/fm_app_stubs.c
  stubs/fm_child_stubs.c
  stubs/fm_tbl_stubs.c
)

# Link with the cfe core stubs and unit test assert libs
target_link_libraries(coverage-fm_internal-stubs ut_core_api_stubs ut_assert)

# Include and expose unit test utilities, fsw/inc, and fsw/src includes
target_include_directories(coverage-fm_internal-stubs PUBLIC utilities)
target_include_directories(coverage-fm_internal-stubs PUBLIC $<TARGET_PROPERTY:fm,INCLUDE_DIRECTORIES>)

# This permits all source files under this directory scope to reference internal
# headers in the "fsw/src" directory.  This is OK because it is a white-box test,
# normally these headers should not be referenced outside the module.
include_directories(../fsw/src)

# Generate a dedicated "testrunner" executable for each test file
# Accomplish this by cycling through all the app's source files, there must be
# a *_tests file for each
foreach(SRCFILE ${APP_SRC_FILES})

    # Get the base sourcefile name as a module name without path or the
    # extension, this will be used as the base name of the unit test file.
    get_filename_component(UNIT_NAME "${SRCFILE}" NAME_WE)

    # Use the module name to make the test name by adding _tests to the end
    set(TESTS_NAME "${UNIT_NAME}_tests")

    # Make the test sourcefile name with unit test path and extension
    set(TESTS_SOURCE_FILE "${PROJECT_SOURCE_DIR}/unit-test/${TESTS_NAME}.c")

    # Create the coverage test executable
    add_cfe_coverage_test(fm "${UNIT_NAME}" "${TESTS_SOURCE_FILE}" "${CFS_FM_SOURCE_DIR}/${SRCFILE}")

    # Add dependency to utilities and internal stubs
    add_cfe_coverage_dependency(fm "${UNIT_NAME}" fm_internal)

endforeach()
