# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventQueuePush_utest
  SOURCES
    case_err_deepcopy.c
    case_err_mutex.c
    case_err_param.c
    case_err_vecpush.c
    case_success.c
    elosEventQueuePush_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    mock_elos_event_static
    cmocka_mocks::mock_libc
    safu::mock_safu
)
