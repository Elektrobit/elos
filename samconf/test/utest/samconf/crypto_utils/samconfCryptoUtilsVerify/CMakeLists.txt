# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_samconfCryptoUtilsVerify_utest
  SOURCES
    case_error_mismatch.c
    case_error_null.c
    case_success.c
    samconfCryptoUtilsVerify_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_openssl
)
