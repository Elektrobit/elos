# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventfilter_elosEventFilterExecute_utest
  SOURCES
    case_err_element.c
    case_err_param.c
    case_success_hardwareid.c
    case_success_element.c
    elosEventFilterExecute_utest.c
    elosEventFilterExecute_utils.c
  LIBRARIES
    eventfilter_static_weak
)
