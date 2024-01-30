# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_libelos_elosMessageLogFindEvent_utest
  SOURCES
    case_err_connection.c
    case_err_message.c
    case_exterr_event_vector_initialize.c
    case_exterr_event_vector_to_json_arr.c
    case_exterr_log_aggregator_find_events.c
    case_exterr_response_create_failed.c
    case_exterr_rulefrmmsg_get_string.c
    case_exterr_rulefrmmsg_strdup.c
    case_exterr_rulefrmmsg_tokener_parse.c
    case_success.c

    elosMessageLogFindEvent_utest.c
  LIBRARIES
    scanner_manager_static_weak
    elos_event_static_weak
    mock_elos_event_static
    mock_log_aggregator_static
    mock_message_handler_static
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
