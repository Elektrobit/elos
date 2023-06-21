# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterExecuteResult_utest
  SOURCES
    case_err_param.c
    case_success_match_false.c
    case_success_match_true.c
    elosRpnFilterExecuteResult_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
