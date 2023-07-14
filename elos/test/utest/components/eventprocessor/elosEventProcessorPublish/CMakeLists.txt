# SPDX-License-Identifier: MIT
find_package(safu)

create_unit_test(
  NAME
    test_eventprocessor_elosEventProcessorPublish_utest
  SOURCES
    case_success.c
    case_err_param.c
    case_process_failed.c
    elosEventProcessorPublish_utest.c
  LIBRARIES
    eventprocessor_static
    safu::mock_safu
)
