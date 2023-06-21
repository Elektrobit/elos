# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_common_safuGetEnvOr_utest
  SOURCES
    case_err_key_is_null.c
    case_success_key_exists.c
    case_success_key_not_exists.c
    safuGetEnvOr_utest.c
  LIBRARIES
    safu_static
    cmocka_mocks::mock_libc
)
