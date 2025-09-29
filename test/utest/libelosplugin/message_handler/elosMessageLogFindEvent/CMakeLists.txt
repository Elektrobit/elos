# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(json-c)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageLogFindEvent_utest
  SOURCES
    case_err_find_event_fail.c
    case_err_null_conn.c
    case_err_null_msg.c
    case_err_msg_length_null.c
    case_err_invalid_request_filter.c
    case_no_request_time_success.c
    case_truncated_success.c
    case_untruncated_success.c
    elosMessageLogFindEvent_utest.c
  LIBRARIES
    mock_libelosplugin_static
    libelosplugin_static_weak
    elos_event_static_weak
)
