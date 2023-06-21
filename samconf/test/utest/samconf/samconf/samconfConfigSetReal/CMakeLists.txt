# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_event_samconfConfigSetReal_utest
  SOURCES
    case_error_config_null.c
    case_success_set_real.c
    samconfConfigSetReal_utest.c
  LIBRARIES
    samconf_static
)
