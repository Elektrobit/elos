# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_common_safuStringIsEmpty_utest
  SOURCES
    case_success_string_is_empty.c
    safuStringIsEmpty_utest.c
  LIBRARIES
    safu_static
    cmocka_mocks::mock_libc
)
