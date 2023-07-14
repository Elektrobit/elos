# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_rpnfilter_elosRpnFilterBuilderTokenize_utest
  SOURCES
    case_err_internal.c
    case_err_param.c
    case_err_string.c
    case_success_command.c
    case_success_location.c
    case_success_number.c
    case_success_string.c
    case_success.c
    elosRpnFilterBuilderTokenize_utest.c
    elosRpnFilterBuilderTokenize_utils.c
  LIBRARIES
    rpnfilter_static_weak
)
