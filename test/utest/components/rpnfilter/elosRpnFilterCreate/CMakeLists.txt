# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterCreate_utest
  SOURCES
    case_err_filter.c
    case_err_param.c
    case_success.c
    elosRpnFilterCreate_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
