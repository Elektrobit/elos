# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventQueueManagerEntryDelete_utest
  SOURCES
    case_err_param.c
    case_err_removebyid.c
    case_err_wrong_id.c
    case_success.c
    elosEventQueueManagerEntryDelete_utest.c
  LIBRARIES
    eventprocessor_static
    safu::mock_safu
)
