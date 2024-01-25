project(CFS_FM C)

# Type of data compression to link in.  Actual routines to implement compression
# are expected to be in a separately loaded library.  Bindings are available for:
#  FALSE or OFF: Do not include compression
#  TRUE or ON: Use default compression for the platform (TBD)
#  CFS_FS_LIB: historical unzip implementation from older versions of CFE FS (deprecated)
#  ZLIB: Use inflate/deflate API from zlib (http://zlib.net) (not yet implemented)
set(FM_INCLUDE_COMPRESSION FALSE CACHE STRING "Type of data compression/decompression features to include in FM")
set(FM_DEPENDENCY_LIST)
set(FM_OPTION_SRC_FILES)

set(APP_SRC_FILES
  fsw/src/fm_cmd_utils.c
  fsw/src/fm_app.c
  fsw/src/fm_cmds.c
  fsw/src/fm_child.c
  fsw/src/fm_dispatch.c
  fsw/src/fm_tbl.c
)

# If compression features are enabled, choose the adapter based on the selected implementation
if (FM_INCLUDE_COMPRESSION)

  if (FM_INCLUDE_COMPRESSION STREQUAL ZLIB)
    # Using a properly-maintained external implementation should be preferred
    # This may be the default in a future release.
    list(APPEND FM_OPTION_SRC_FILES fsw/src/fm_compression_zlib.c)
  else()
    # Older versions of FM used a decompression implemented in CFS FS, so this is
    # what will be assumed if FM_INCLUDE_COMPRESSION is set to a simple
    # boolean "ON" or "TRUE".  This code now resides in an external "FS_Lib".
    list(APPEND FM_DEPENDENCY_LIST fs_lib)
    list(APPEND FM_OPTION_SRC_FILES fsw/src/fm_compression_fslib.c)
  endif()

else()

  # FM_INCLUDE_COMPRESSION set to "OFF" or "FALSE" - compression features are not enabled
  list(APPEND FM_OPTION_SRC_FILES fsw/src/fm_compression_none.c)

endif()

# Create the app module
add_cfe_app(fm ${APP_SRC_FILES} ${FM_OPTION_SRC_FILES})

# This permits direct access to public headers in the fsw/inc directory
target_include_directories(fm PUBLIC fsw/inc)

# Add dependencies based on the type of compression algorithm selected (if any)
if (FM_DEPENDENCY_LIST)
  add_cfe_app_dependency(fm ${FM_DEPENDENCY_LIST})
endif()

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
