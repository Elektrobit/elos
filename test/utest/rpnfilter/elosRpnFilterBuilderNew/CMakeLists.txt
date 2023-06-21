# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterBuilderNew_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosRpnFilterBuilderNew_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
