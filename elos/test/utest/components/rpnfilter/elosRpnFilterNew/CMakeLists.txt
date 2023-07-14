# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterNew_utest
  SOURCES
    case_err_alloc.c
    case_err_init.c
    case_err_param_null.c
    case_success.c
    elosRpnFilterNew_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
