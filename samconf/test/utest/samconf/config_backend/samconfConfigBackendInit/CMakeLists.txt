# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_samconfConfigBackendInit_utest
  SOURCES
    case_success.c
    samconfConfigBackendInit_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    safu::mock_safu
    cmocka_mocks::mock_libc
)
