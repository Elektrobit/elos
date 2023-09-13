# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_logger_elosLoggerGetDefaultLogger_utest
  SOURCES
    case_success.c
    case_err_func.c
    elosLoggerGetDefaultLogger_utest.c
  LIBRARIES
    mock_logger_static
    mock_eventbuffer_static
    logger_static_weak
)
