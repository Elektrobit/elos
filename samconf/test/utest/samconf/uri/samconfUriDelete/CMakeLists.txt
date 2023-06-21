# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_samconfUriDelete_utest
  SOURCES
    case_success.c
    case_memberserror.c
    samconfUriDelete_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    cmocka_mocks::mock_libc
)
