cmake_minimum_required(VERSION 1.6.4)
project(CFS_FM C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(${cfs_lib_MISSION_DIR}/fsw/public_inc)
#include_directories(${fs_lib_MISSION_DIR}/fsw/public_inc)
#include_directories(${fs_lib_MISSION_DIR}/fsw/src)
include_directories(${cfs_lib_MISSION_DIR}/fsw/src)

message(STATUS "cfs_lib loc: ${cfs_lib_MISSION_DIR}")
#message(STATUS "fs_lib loc : ${fs_lib_MISSION_DIR}")

# Uncomment the below include if decompress functionality is 
# enabled in FM
# include_directories(${fs_lib_MISSION_DIR}/fsw/public_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(fm ${APP_SRC_FILES})

include_directories(fsw/src)
aux_source_directory(fsw/tables APP_TABLE_FILES)
add_cfe_tables(fm ${APP_TABLE_FILES})
if (ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)
