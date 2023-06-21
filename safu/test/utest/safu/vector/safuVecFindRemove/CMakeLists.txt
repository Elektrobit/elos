# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecFindRemove_utest
  SOURCES
    case_err_uninitialized_vec.c
    case_err_no_match_element.c
    case_err_no_match_func.c
    case_element_not_found.c
    case_success.c
    case_success_multi_match.c
    case_empty_vec.c
    ../vector_helper.c
    safuVecFindRemove_utest.c
  LIBRARIES
    safu_static
)
