# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_common_safuJsonAddObject_utest
  SOURCES
    case_success_array.c
    case_success_object.c
    case_err_array.c
    case_err_object.c
    case_err_null_key_object.c
    case_err_string_index_array.c
    safuJsonAddObject_utest.c
  LIBRARIES
    safu_weak
    cmocka_mocks::mock_jsonc
)
