# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_config_samconfConfigDeleteMembers_utest
  SOURCES
    case_success.c
    case_exterr_configdelete.c
    case_err_null_configuration.c
    samconfConfigDeleteMembers_utest.c
  LIBRARIES
    mock_samconf_static
)
