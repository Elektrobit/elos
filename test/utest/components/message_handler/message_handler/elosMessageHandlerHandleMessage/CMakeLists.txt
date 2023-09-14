# SPDX-License-Identifier: MIT
find_package(safu)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_message_handler_elosMessageHandlerHandleMessage_utest
  SOURCES
    case_err_connection.c
    case_exterr_first_allocMem.c
    case_err_read_header_failed.c
    case_exterr_second_allocMem.c
    case_err_read_payload_failed.c
    case_err_invalid_message_id.c
    case_err_message_function_return_failure.c
    case_success.c
    elosMessageHandlerHandleMessage_utest.c
  LIBRARIES
    scanner_manager_static_weak
    logger_static_weak
    mock_message_handler_static
    cmocka_mocks::mock_libc
    safu::mock_safu
)
