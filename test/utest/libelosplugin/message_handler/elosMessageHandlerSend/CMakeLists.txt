# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_message_handler_elosMessageHandlerSend_utest
  SOURCES
    case_success.c
    case_exterr_allocMem.c
    case_err_bytes_lt_zero.c
    case_err_bytes_lt_msgLen.c
    elosMessageHandlerSend_utest.c
  LIBRARIES
    libelosplugin_static
    elos_event_static_weak
    safu::mock_safu
)
