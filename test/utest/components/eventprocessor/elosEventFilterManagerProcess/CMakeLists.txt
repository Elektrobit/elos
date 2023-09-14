# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterManagerProcess_utest
  SOURCES
    case_err_efnv.c
    case_err_param.c
    case_success.c
    elosEventFilterManagerProcess_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
)
