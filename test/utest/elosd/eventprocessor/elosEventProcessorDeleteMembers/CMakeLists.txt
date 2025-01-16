# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventprocessor_elosEventProcessorDeleteMembers_utest
  SOURCES
    case_err_component.c
    case_null_param.c
    case_success.c
    elosEventProcessorDeleteMembers_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
)
