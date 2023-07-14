# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_processfilter_elosProcessFilterExecute_utest
  SOURCES
    case_err_element.c
    case_err_param.c
    case_success_element.c
    elosProcessFilterExecute_utest.c
    elosProcessFilterExecute_utils.c
  LIBRARIES
    processfilter_static_weak
)
