# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_eventdispatcher_elosEventDispatcherDeleteMembers_utest
  SOURCES
    case_err_mutex.c
    case_err_param.c
    case_success.c
    elosEventDispatcherDeleteMembers_utest.c
  LIBRARIES
    eventdispatcher_static
    safu::mock_safu
    cmocka_mocks::mock_libc
)
