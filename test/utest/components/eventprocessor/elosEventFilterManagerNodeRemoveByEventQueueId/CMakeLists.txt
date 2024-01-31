# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterManagerNodeRemoveByEventQueueId_utest
  SOURCES
    case_err_param.c
    case_err_remove.c
    case_success.c
    elosEventFilterManagerNodeRemove_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    safu::mock_safu
)
