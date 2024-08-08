# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosSendJsonMessage_utest
  SOURCES
    case_err_call.c
    case_err_param.c
    case_success.c
    elosSendJsonMessage_utest.c
  LIBRARIES
    mock_libelos_static
    safu::mock_safu
    cmocka_mocks::mock_jsonc
)
