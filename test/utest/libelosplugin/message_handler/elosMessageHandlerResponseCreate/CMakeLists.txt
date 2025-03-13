# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageHandlerResponseCreate_utest
  SOURCES
    case_success_null_string.c
    case_success_err_string.c
    case_exterr_json_object.c
    case_exterr_add_object.c
    case_exterr_add_new_string.c
    elosMessageHandlerResponseCreate_utest.c
  LIBRARIES
    libelosplugin_static
    elos_event_static_weak
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
