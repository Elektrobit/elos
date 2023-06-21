# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_event_samconfConfigNew_utest
  SOURCES
    case_success.c
    case_err_null_config.c
    case_exterr_allocMem.c
    samconfConfigNew_utest.c
  LIBRARIES
    samconf_static
    safu::safu
    cmocka_mocks::mock_libc
    safu::mock_safu
)
