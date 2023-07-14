# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_event_elosEventIsDateEmpty_utest
  SOURCES
    case_date_is_empty.c
    case_date_is_not_empty.c
    elosEventIsDateEmpty_utest.c
  LIBRARIES
    elos_event_static
)
