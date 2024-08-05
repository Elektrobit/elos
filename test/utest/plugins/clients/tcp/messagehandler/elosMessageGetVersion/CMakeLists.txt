# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosMessageGetVersion_utest
  SOURCES
    case_exterr_new_object.c
    case_exterr_add_new_string.c
    case_success.c
    elosMessageGetVersion_utest.c
  LIBRARIES
    connectionmanager_static
    mock_message_handler_static
    elos_event_static_weak
    version
    safu::mock_safu
)
