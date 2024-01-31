# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventQueueNew_utest
  SOURCES
    case_success.c
    case_err_param.c
    case_allocation_failed.c
    case_initialize_failed.c
    elosEventQueueNew_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    mock_elos_event_static
    cmocka_mocks::mock_libc
    safu::mock_safu
)
