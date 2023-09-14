# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterExecute_utest
  SOURCES
    case_err_param.c
    case_success_arithmetic.c
    case_success_operation.c
    case_success_string.c
    elosRpnFilterExecute_utest.c
    elosRpnFilterExecute_utils.c
  LIBRARIES
    rpnfilter_static
)
