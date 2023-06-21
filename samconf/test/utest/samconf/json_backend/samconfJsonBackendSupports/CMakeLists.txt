# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
  test_samconfJsonBackendSupports_utest
  SOURCES
    case_success_file.c
    case_invalid_file_ext.c
    samconfJsonBackendSupports_utest.c
  LIBRARIES
    samconf_static
    safu::mock_safu
    cmocka_mocks::mock_jsonc
    cmocka_mocks::mock_libc
)
