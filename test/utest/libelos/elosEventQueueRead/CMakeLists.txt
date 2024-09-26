# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosEventQueueRead_utest
  SOURCES
    case_err_conversion.c
    case_err_exchange.c
    case_err_param.c
    case_err_request.c
    case_success.c
    elosEventQueueRead_utest.c
  LIBRARIES
    mock_libelos_static
    mock_elos_event_static
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
