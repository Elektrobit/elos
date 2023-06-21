# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecGet_utest
  SOURCES
    ../vector_helper.c
    case_success.c
    case_err_out_of_bounds.c
    case_err_uninitialized.c
    safuVecGet_utest.c
  LIBRARIES
    safu_static
)
