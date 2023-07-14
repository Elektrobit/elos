# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterDelete_utest
  SOURCES
    case_err_param_null.c
    case_success.c
    elosRpnFilterDelete_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
