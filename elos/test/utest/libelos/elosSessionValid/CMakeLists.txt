# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_libelos_elosSessionValid_utest
  SOURCES
    case_success.c
    elosSessionValid_utest.c
  LIBRARIES
    libelos_static_weak
    cmocka_mocks::mock_libc
)
