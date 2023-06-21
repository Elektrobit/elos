# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_processfilter_elosProcessFilterBuilderCompile_utest
  SOURCES
    case_err_param.c
    case_success_location.c
    elosProcessFilterBuilderCompile_utest.c
    elosProcessFilterBuilderCompile_utils.c
  LIBRARIES
    processfilter_static_weak
)
