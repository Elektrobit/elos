# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_eventSource_elosEventSourceDelete_utest
  SOURCES
    case_initialized_success.c
    case_uninitialized_success.c
    elosEventSourceDelete_utest.c
  LIBRARIES
    elos_event_static
)
