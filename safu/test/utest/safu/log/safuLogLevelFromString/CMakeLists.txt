# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuLogLevelFromString_utest
  SOURCES
    case_success.c
    case_err_invalid_log_level.c
    safuLogLevelFromString_utest.c
  LIBRARIES
    safu_static
)
