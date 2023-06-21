# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_common_safuJsonAddNewTimestamp_utest
  SOURCES
    case_success_array.c
    case_success_object.c
    case_err_new_array_array.c
    case_err_new_array_object.c
    case_err_add_array_array.c
    case_err_add_array_object.c
    case_err_create_sec_array.c
		case_err_create_sec_object.c
    case_err_create_nsec_array.c
		case_err_create_nsec_object.c
		case_err_insert_sec_array.c
		case_err_insert_sec_object.c
		case_err_insert_nsec_array.c
		case_err_insert_nsec_object.c
		case_err_no_timespec_array.c
		case_err_no_timespec_object.c
    safuJsonAddNewTimestamp_utest.c
  LIBRARIES
    safu_weak
    cmocka_mocks::mock_jsonc
)
