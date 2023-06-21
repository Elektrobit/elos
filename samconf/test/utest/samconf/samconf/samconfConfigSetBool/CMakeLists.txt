# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_event_samconfConfigSetBool_utest
  SOURCES
    case_error_config_null.c
    case_success_set_bool.c
    samconfConfigSetBool_utest.c
  LIBRARIES
    samconf_static
)
