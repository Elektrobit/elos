# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventprocessor_elosEventProcessorQueueRead_utest
  SOURCES
    case_err_func.c
    case_err_param.c
    case_success.c
    elosEventProcessorQueueRead_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
)
