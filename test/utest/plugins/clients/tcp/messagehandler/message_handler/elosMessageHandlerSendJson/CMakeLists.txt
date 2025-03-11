# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageHandlerSendJson_utest
  SOURCES
    case_success.c
    case_exterr_json_object.c
    case_exterr_send.c
    elosMessageHandlerSendJson_utest.c
  LIBRARIES
    mock_message_handler_static
    libelosplugin_static_weak
    elos_event_static_weak
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
