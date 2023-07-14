# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterBuilderFinalize_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosRpnFilterBuilderFinalize_utest.c
    elosRpnFilterBuilderFinalize_utils.c
  LIBRARIES
    rpnfilter_static_weak
)
