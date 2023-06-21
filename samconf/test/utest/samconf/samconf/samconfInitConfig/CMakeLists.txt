# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_event_samconfInitConfig_utest
  SOURCES
    case_success.c
    samconfInitConfig_utest.c
  LIBRARIES
    samconf_static
)
