# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuLogLevelToString_utest
  SOURCES
    case_success.c
    case_err_invalid_log_level.c
    safuLogLevelToString_utest.c
  LIBRARIES
    safu_static
)
