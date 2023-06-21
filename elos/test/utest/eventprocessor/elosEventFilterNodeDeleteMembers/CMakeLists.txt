# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeDeleteMembers_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosEventFilterNodeDeleteMembers_utest.c
  LIBRARIES
    eventprocessor_static
)
