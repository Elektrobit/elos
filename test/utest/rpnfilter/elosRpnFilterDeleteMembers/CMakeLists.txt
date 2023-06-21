# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterDeleteMembers_utest
  SOURCES
    case_err_param_null.c
    case_success.c
    elosRpnFilterDeleteMembers_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
