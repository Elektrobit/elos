# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
  test_config_samconfConfigInit_utest
  SOURCES
    case_success.c
    case_err_null_config.c
    samconfConfigInit_utest.c
  LIBRARIES
    samconf_static

)
