# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosProcessFilterCreate_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosProcessFilterCreate_utest.c
  LIBRARIES
    processfilter_static_weak
)
