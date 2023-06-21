# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_config_samconfConfigGetBool_utest
  SOURCES
    case_success_entry_found.c
    case_success_entry_not_found.c
    case_success_node_not_bool.c
    case_exterr_configget.c
    case_err_root_null.c
    case_err_path_null.c
    case_err_result_null.c
    samconfConfigGetBool_utest.c
  LIBRARIES
    mock_samconf_static
)
