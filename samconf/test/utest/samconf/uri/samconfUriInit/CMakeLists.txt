# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_samconfUriInit_utest
  SOURCES
    case_success.c
    samconfUriInit_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    cmocka_mocks::mock_libc
)
