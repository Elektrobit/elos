# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuLogFunc_utest
  SOURCES
    case_success.c
    case_err_integersize.c
    safuLogFunc_utest.c
  LIBRARIES
    safu_static
)
