# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_logger_elosLoggerDeleteMembers_utest
  SOURCES
    case_success.c
    case_err_func.c
    case_err_param.c
    elosLoggerDeleteMembers_utest.c
  LIBRARIES
    logger_static
    mock_eventbuffer_static
)
