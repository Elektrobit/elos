# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosEventUnsubscribe_utest
  SOURCES
    case_err_communication.c
    case_err_param.c
    case_err_request.c
    case_success.c
    elosEventUnsubscribe_utest.c
  LIBRARIES
    mock_libelos_static
    safu::mock_safu
    cmocka_mocks::mock_jsonc
)
