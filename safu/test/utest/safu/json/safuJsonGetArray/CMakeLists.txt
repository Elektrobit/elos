# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_common_safuJsonGetArray_utest
  SOURCES
    case_success_array.c
    case_success_object.c
    case_success_no_len_array.c
    case_success_no_len_object.c
    case_err_no_array_array.c
    case_err_no_array_object.c
    case_err_get_null_array.c
    case_err_get_null_object.c
    safuJsonGetArray_utest.c
  LIBRARIES
    safu_weak
)
