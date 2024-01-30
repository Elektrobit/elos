# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeProcess_utest
  SOURCES
    case_err_iter.c
    case_err_param.c
    case_success_with_vstack.c
    case_success.c
    elosEventFilterNodeProcess_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    mock_eventfilter_static
    safu::mock_safu
)
