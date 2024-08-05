# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosDisconnect_utest
  SOURCES
    case_exterr_close.c
    case_success.c
    elosDisconnect_utest.c
  LIBRARIES
    libelos_static_weak
    cmocka_mocks::mock_libc
)
