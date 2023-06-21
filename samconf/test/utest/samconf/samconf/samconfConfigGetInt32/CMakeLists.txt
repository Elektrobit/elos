# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_config_samconfConfigGetInt32_utest
  SOURCES
    case_success_entry_found.c
    case_success_entry_not_found.c
    case_success_node_not_int32.c
    case_exterr_configget.c
    case_err_root_null.c
    case_err_path_null.c
    case_err_result_null.c
    samconfConfigGetInt32_utest.c
  LIBRARIES
    mock_samconf_static
)
