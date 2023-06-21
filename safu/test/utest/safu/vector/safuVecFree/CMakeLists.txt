# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecFree_utest
  SOURCES
    ../vector_helper.c
    case_success.c
    case_uninitialized.c
    case_err_null.c
    safuVecFree_utest.c
  LIBRARIES
    safu_static
)
