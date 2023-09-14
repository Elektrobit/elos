# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventfilter_elosEventFilterBuilderCompile_utest
  SOURCES
    case_err_param.c
    case_success_location.c
    elosEventFilterBuilderCompile_utest.c
    elosEventFilterBuilderCompile_utils.c
  LIBRARIES
    eventfilter_static_weak
)
