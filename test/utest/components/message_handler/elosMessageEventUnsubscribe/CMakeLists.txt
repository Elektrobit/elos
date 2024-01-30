# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageEventUnsubscribe_utest
  SOURCES
    case_err_filter_delete.c
    case_err_queue_delete.c
    case_err_request.c
    case_err_response_create.c
    case_err_response_send.c
    case_success.c
    elosMessageEventUnsubscribe_utest.c
  LIBRARIES
    scanner_manager_static_weak
    cmocka_mocks::mock_jsonc
    cmocka_mocks::mock_libc
    safu::mock_safu
    mock_elos_event_static
    mock_message_handler_static
    mock_eventprocessor_static
)
