# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecFind_utest
  SOURCES
    case_err_no_data_vec.c
    case_err_no_match_func.c
    case_err_no_result_memory.c
    case_err_start_out_of_bounds.c
    case_element_not_found.c
    case_success.c
    case_empty_vec.c
    ../vector_helper.c
    safuVecFind_utest.c
  LIBRARIES
    safu_static
)
