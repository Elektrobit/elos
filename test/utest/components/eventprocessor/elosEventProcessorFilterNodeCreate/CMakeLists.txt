# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventProcessorFilterNodeCreate_utest
  SOURCES
    case_success.c
    case_err_param.c
    case_create_failed.c
    elosEventProcessorFilterNodeCreate_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    safu::mock_safu
)
