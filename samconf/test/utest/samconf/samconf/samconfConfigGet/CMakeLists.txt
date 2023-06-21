# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_config_samconfConfigGet_utest
  SOURCES
    case_ext_err_strdup.c
    case_err_null_result.c
    case_err_null_path.c
    case_err_null_root.c
    case_err_root_has_no_child.c
    case_success_root_child_count_zero.c
    case_success_entry_found.c
    case_success_entry_not_found.c
    case_success_result_non_null.c
    samconfConfigGet_utest.c
  LIBRARIES
    samconf_static
    cmocka_mocks::mock_libc
)
