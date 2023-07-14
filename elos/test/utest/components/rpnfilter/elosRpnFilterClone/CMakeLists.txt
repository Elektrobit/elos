# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterClone_utest
  SOURCES
    case_err_alloc.c
    case_err_deepcopy.c
    case_err_param_null.c
    case_success.c
    elosRpnFilterClone_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
