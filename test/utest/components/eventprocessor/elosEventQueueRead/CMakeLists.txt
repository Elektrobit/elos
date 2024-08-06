# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventQueueRead_utest
  SOURCES
    case_err_func.c
    case_err_mutex.c
    case_err_param.c
    case_success.c
    elosEventQueueRead_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    mock_elos_event_static
    cmocka_mocks::mock_libc
)
