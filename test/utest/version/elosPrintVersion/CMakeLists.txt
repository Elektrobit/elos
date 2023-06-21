# SPDX-License-Identifier: MIT

# Remove false positives caused by -fanalyzer not detecting cmockas "assert_non_null()" as a null pointer check
SET_SOURCE_FILES_PROPERTIES(case_null_prefix.c PROPERTIES COMPILE_FLAGS "-Wno-analyzer-possible-null-argument")
SET_SOURCE_FILES_PROPERTIES(case_with_prefix.c PROPERTIES COMPILE_FLAGS "-Wno-analyzer-possible-null-argument")

create_unit_test(
  NAME
    test_version_elosPrintVersion_utest
  SOURCES
    elosPrintVersion_utest.c
    case_null_prefix.c
    case_with_prefix.c
  LIBRARIES
    version
)
