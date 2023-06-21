# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecElements_utest
  SOURCES
    ../vector_helper.c
    case_success.c
    case_err_uninitialized.c
    safuVecElements_utest.c
  LIBRARIES
    safu_static
)
