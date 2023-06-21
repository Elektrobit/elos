# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_event_samconfConfigSetInt_utest
  SOURCES
    case_error_config_null.c
    case_success_set_int.c
    samconfConfigSetInt_utest.c
  LIBRARIES
    samconf_static
)
