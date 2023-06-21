# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuLogGetStreamLevel_utest
  SOURCES
    case_success.c
    safuLogGetStreamLevel_utest.c
  LIBRARIES
    safu_static
)
