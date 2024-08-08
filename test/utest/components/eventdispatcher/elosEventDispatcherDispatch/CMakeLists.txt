# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_eventdispatcher_elosEventDispatcherDispatch_utest
  SOURCES
    case_err_func.c
    case_err_mutex.c
    case_err_param.c
    case_success.c
    elosEventDispatcherDispatch_utest.c
  LIBRARIES
    mock_eventbuffer_static
    mock_eventprocessor_static
    eventdispatcher_static_weak
    safu::mock_safu
    cmocka_mocks::mock_libc

)
