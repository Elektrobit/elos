# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuLogSetFilterStr_utest
  SOURCES
    case_success.c
    case_err_to_long_filter.c
    safuLogSetFilterStr_utest.c
  LIBRARIES
    safu_static
)
