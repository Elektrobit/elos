# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageEventSubscribe_utest
  SOURCES
    case_complete_failure.c
    case_filter_node_null.c
    case_filter_allocate_failed.c
    case_no_filter.c
    case_no_json.c
    case_no_loop.c
    case_no_string.c
    case_queue_id_null.c
    case_success.c
    case_strdup_failed.c
    case_response_new_array_failed.c
    case_response_new_int_failed.c
    case_response_send_failed.c
    elosMessageEventSubscribe_utest.c
  LIBRARIES
    scanner_manager_static_weak
    cmocka_mocks::mock_jsonc
    cmocka_mocks::mock_libc
    safu::mock_safu
    mock_elos_event_static
    mock_message_handler_static
    mock_eventprocessor_static
)
