# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventQueueManagerEntryNew_utest
  SOURCES
    case_err_param.c
    case_err_pushbyparam.c
    case_success.c
    elosEventQueueManagerEntryNew_utest.c
  LIBRARIES
    eventprocessor_static
    mock_eventprocessor
    safu::mock_safu
)
