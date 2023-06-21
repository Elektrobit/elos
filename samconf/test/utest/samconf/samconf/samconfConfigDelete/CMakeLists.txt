# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_config_samconfConfigDelete_utest
  SOURCES
    case_success.c
    case_err_null_configuration.c
    samconfConfigDelete_utest.c
  LIBRARIES
    mock_samconf_static
)
