# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterManagerInitialize_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosEventFilterManagerInitialize_utest.c
  LIBRARIES
    eventprocessor_static
)
