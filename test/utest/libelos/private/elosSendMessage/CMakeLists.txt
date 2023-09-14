# SPDX-License-Identifier: MIT
find_package(safu)

create_unit_test(
  NAME
    test_libelos_elosSendMessage_utest
  SOURCES
    case_err_param.c
    case_err_send.c
    case_success.c
    elosSendMessage_utest.c
  LIBRARIES
    mock_libelos_static
    safu::mock_safu
)
