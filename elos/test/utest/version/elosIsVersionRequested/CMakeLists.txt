# SPDX-License-Identifier: MIT

# Disable compiler warning to test the very scenario the warning is about 
SET_SOURCE_FILES_PROPERTIES(case_null_param.c PROPERTIES COMPILE_FLAGS "-Wno-nonnull -Wno-analyzer-null-argument")

create_unit_test(
  NAME
    test_version_elosIsVersionRequested_utest
  SOURCES
    elosIsVersionRequested_utest.c
    case_null_param.c
    case_empty_request.c
    case_no_request.c
    case_short_request.c
    case_long_request.c
  LIBRARIES
    version
)
