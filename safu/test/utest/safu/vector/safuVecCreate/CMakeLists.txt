# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_vector_safuVecCreate_utest
  SOURCES
    case_success.c
    case_exterr_malloc.c
    case_err_wrong_parameter.c
    safuVecCreate_utest.c
  LIBRARIES
    safu_static
    cmocka_mocks::mock_libc
)
