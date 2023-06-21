# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecSet_utest
  SOURCES
    ../vector_helper.c
    case_success.c
    case_err_out_of_bounds.c
    case_err_uninitialized.c
    safuVecSet_utest.c
  LIBRARIES
    safu_static
)
