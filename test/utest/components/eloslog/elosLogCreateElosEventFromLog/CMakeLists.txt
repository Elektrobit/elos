# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_eloslog_elosLogCreateElosEventFromLog_utest
  SOURCES
    case_success.c
    case_err_param_event.c
    elosLogCreateElosEventFromLog_utest.c
  LIBRARIES
    elos_event_static_weak
    eloslog_static_weak
    mock_elos_event_static
    safu::mock_safu
)
