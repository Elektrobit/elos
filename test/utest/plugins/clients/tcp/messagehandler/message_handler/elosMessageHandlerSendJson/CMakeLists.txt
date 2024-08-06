# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageHandlerSendJson_utest
  SOURCES
    case_success.c
    case_exterr_json_object.c
    case_exterr_send.c
    elosMessageHandlerSendJson_utest.c
  LIBRARIES
    messagehandler_static_weak
    mock_message_handler_static
    elos_event_static_weak
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
