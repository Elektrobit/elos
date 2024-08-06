# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_eventdispatcher_elosEventDispatcherStop_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosEventDispatcherStop_utest.c
  LIBRARIES
    eventdispatcher_static
    safu::mock_safu
    cmocka_mocks::mock_libc
)
