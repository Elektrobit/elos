# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeVectorProcess_utest
  SOURCES
    case_err_iter.c
    case_err_param.c
    case_success_with_vstack.c
    case_success.c
    elosEventFilterNodeVectorProcess_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    safu::mock_safu
)
