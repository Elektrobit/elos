# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_logger_elosLog_utest
  SOURCES
    case_success.c
    case_logger_uninitialized.c
    elosLog_utest.c
  LIBRARIES
    mock_logger_static
    mock_eventbuffer_static
    logger_static_weak
)
