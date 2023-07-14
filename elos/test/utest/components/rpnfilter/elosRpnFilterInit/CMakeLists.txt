# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterInit_utest
  SOURCES
    case_err_param_null.c
    case_success.c
    elosRpnFilterInit_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
