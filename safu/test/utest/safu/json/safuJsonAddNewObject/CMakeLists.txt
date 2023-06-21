# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_common_safuJsonAddNewObject_utest
  SOURCES
    case_success_array.c
    case_success_object.c
    case_err_new_object_failed_array.c
    case_err_new_object_failed_object.c
    case_err_array.c
    case_err_object.c
    ../safuJsonAddObject_mock.c
    safuJsonAddNewObject_utest.c
  LIBRARIES
    safu_weak
    cmocka_mocks::mock_jsonc
)
