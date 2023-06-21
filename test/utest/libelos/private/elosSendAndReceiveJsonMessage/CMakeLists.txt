# SPDX-License-Identifier: MIT
find_package(safu)

create_unit_test(
  NAME
    test_libelos_elosSendAndReceiveJsonMessage_utest
  SOURCES
    case_err_call.c
    case_err_param.c
    case_success.c
    elosSendAndReceiveJsonMessage_utest.c
  LIBRARIES
    mock_libelos_static
    safu::mock_safu
)
