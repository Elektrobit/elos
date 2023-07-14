# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterBuilderDelete_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosRpnFilterBuilderDelete_utest.c
  LIBRARIES
    rpnfilter_static_weak
)
