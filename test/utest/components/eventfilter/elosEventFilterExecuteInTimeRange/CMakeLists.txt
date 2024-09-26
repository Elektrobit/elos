# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  	test_eventfilter_elosEventFilterExecuteInTimeRange_utest
  SOURCES
    case_success_in_range.c
    case_success_out_of_range.c
    elosEventFilterExecuteInTimeRange_utest.c
    elosEventFilterExecuteInTimeRange_utils.c
  LIBRARIES
    eventfilter_static_weak
)
