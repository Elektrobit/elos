# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_samconfBase64Decode_utest
  SOURCES
    case_success.c
    case_allocerror.c
    case_decodeerror.c
    case_paddingerror.c
    samconfBase64Decode_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    safu::mock_safu
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_openssl
)
