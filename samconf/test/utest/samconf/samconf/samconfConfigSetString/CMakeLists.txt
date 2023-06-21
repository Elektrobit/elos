# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
  test_event_samconfConfigSetString_utest
  SOURCES
    case_error_config_null.c
    case_error_value_null.c
    case_exterr_strdup.c
    case_success_set_string.c
    samconfConfigSetString_utest.c
  LIBRARIES
    samconf_static
    safu::mock_safu
    cmocka_mocks::mock_libc
)
