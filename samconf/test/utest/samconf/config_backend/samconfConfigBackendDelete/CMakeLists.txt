# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_samconfConfigBackendDelete_utest
  SOURCES
    case_success.c
    samconfConfigBackendDelete_utest.c
  LIBRARIES
    samconf_static
    cmocka_mocks::mock_libc
)
