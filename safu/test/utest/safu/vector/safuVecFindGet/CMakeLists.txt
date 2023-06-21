# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecFindGet_utest
  SOURCES
    case_err_start_out_of_bounds.c
    case_err_uninitialized_vec.c
    case_err_no_result_memory.c
    case_err_no_match_func.c
    case_err_no_match_data.c
    case_element_not_found.c
    case_success.c
    case_empty_vec.c
    ../vector_helper.c
    safuVecFindGet_utest.c
  LIBRARIES
    safu_static
)
