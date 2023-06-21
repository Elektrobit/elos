# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_samconfConfigBackendNew_utest
  SOURCES
    case_success.c
    case_memerror.c
    case_initerror.c
    samconfConfigBackendNew_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    safu::mock_safu
    cmocka_mocks::mock_libc
)
