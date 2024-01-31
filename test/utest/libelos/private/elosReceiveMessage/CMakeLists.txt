# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosReceiveMessage_utest
  SOURCES
    case_err_allocation.c
    case_err_param.c
    case_err_receive_body.c
    case_err_receive_header.c
    case_success.c
    elosReceiveMessage_utest.c
  LIBRARIES
    mock_libelos_static
    safu::mock_safu
)
