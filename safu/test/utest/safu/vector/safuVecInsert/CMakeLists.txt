# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_vector_safuVecInsert_utest
  SOURCES
    case_consecutive_with_resize.c
    case_consecutive_without_resize.c
    case_front_with_resize.c
    case_front_without_resize.c
    case_with_resize.c
    case_without_resize.c
    case_err_invalid_index.c
    case_exterr_realloc.c
    case_err_null_vector.c
    safuVecInsert_utest.c
  LIBRARIES
    safu_static
    cmocka_mocks::mock_libc
)
