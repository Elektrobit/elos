# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
  test_eventSource_elosEventSourceNew_utest
  SOURCES
    case_success.c
    case_exterr_calloc.c
    elosEventSourceNew_utest.c
  LIBRARIES
    elos_event_static
    cmocka_mocks::mock_libc
)
