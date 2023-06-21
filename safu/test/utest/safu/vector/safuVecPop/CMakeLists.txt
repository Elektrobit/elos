# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_vector_safuVecPop_utest
  SOURCES
    ../vector_helper.c
    case_success.c
    case_empty.c
    case_err_uninitialized.c
    safuVecPop_utest.c
  LIBRARIES
    safu_static
)
