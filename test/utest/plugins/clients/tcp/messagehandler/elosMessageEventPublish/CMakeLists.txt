# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(json-c)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageEventPublish_utest
  SOURCES
    case_success.c
    case_event_deserialize_failed.c
    case_blacklist_filter_success.c
    case_blacklist_filter_create_error.c
    case_null_connection.c
    case_null_message.c
    case_publish_failed.c
    elosMessageEventPublish_utest.c
  LIBRARIES
    connectionmanager_static
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_jsonc
    elos_event_static_weak
    json-c
    mock_elos_event_static
    eventbuffer_static_weak
    mock_eventbuffer_static
    mock_eventfilter_static
    mock_eventprocessor_static
    logger_static_weak
    mock_logger_static
    mock_message_handler_static
    mock_log_aggregator_static
    safu::mock_safu
)
