# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosEventFilterCreate_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosEventFilterCreate_utest.c
  LIBRARIES
    eventfilter_static_weak
)
