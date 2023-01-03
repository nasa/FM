project(CFS_FM C)

# Uncomment the below include if decompress functionality is
# enabled in FM
# include_directories(${fs_lib_MISSION_DIR}/fsw/public_inc)

set(APP_SRC_FILES
  fsw/src/fm_cmd_utils.c
  fsw/src/fm_app.c
  fsw/src/fm_cmds.c
  fsw/src/fm_child.c
  fsw/src/fm_tbl.c
)

# Create the app module
add_cfe_app(fm ${APP_SRC_FILES})

# This permits direct access to public headers in the fsw/src and fsw/inc directories
target_include_directories(fm PUBLIC fsw/src)
target_include_directories(fm PUBLIC fsw/inc)

set(APP_TABLE_FILES
  fsw/tables/fm_monitor.c
)

add_cfe_tables(fm ${APP_TABLE_FILES})

# If UT is enabled, then add the tests from the subdirectory
# Note that this is an app, and therefore does not provide
# stub functions, as other entities would not typically make
# direct function calls into this application.
if(ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif()
