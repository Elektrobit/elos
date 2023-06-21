# SPDX-License-Identifier: MIT
find_package(safu)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_libelos_elosMessageEventQueueRead_utest
  SOURCES
    case_err_request.c
    case_err_queueread.c
    case_err_response_create.c
    case_err_response_send.c
    case_success.c
    elosMessageEventQueueRead_utest.c
  LIBRARIES
    scanner_manager_static_weak
    mock_elos_common_static
    mock_message_handler_static
    mock_eventprocessor_static
    safu::mock_safu
)
