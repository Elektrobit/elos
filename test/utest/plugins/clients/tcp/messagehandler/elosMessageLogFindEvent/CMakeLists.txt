# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(json-c)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageLogFindEvent_utest
  SOURCES
    case_success.c
    elosMessageLogFindEvent_utest.c
  LIBRARIES
    messagehandler_static_weak
    mock_message_handler_static
)
