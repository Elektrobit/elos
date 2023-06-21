# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecGetLast_utest
  SOURCES
    case_success.c
    case_err_empty.c
    case_err_uninitialized.c
    ../vector_helper.c
    safuVecGetLast_utest.c
  LIBRARIES
    safu_static
)
