# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_config_samconfConfigAdd_utest
  SOURCES
    case_success_child_added.c
    case_success_empty_node.c
    case_error_parent_null.c
    case_error_child_null.c
    case_exterr_allocMem.c
    samconfConfigAdd_utest.c
  LIBRARIES
    samconf_static
    safu::mock_safu
    cmocka_mocks::mock_libc
)
