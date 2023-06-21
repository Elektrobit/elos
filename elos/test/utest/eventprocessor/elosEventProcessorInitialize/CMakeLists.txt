# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventprocessor_elosEventProcessorInitialize_utest
  SOURCES
    case_err_component.c
    case_err_param.c
    case_success.c
    elosEventProcessorInitialize_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
)
