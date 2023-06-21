# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecRemove_utest
  SOURCES
    ../vector_helper.c
    case_index_out_of_bounds.c
    case_vec_invalid.c
    case_last_element.c
    case_remove_from_middle.c
    safuVecRemove_utest.c
  LIBRARIES
    safu_static
)
