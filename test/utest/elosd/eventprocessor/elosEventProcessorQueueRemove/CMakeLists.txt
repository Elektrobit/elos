# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventProcessorQueueRemove_utest
  SOURCES
    case_success.c
    case_err_param.c
    case_delete_failed.c
    elosEventProcessorQueueRemove_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    safu::mock_safu
)
