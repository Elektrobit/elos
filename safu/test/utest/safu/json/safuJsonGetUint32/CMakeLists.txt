# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_common_safuJsonGetUint32_utest
  SOURCES
    case_success_array.c
    case_success_object.c
    case_number_out_of_bounds_array.c
    case_number_out_of_bounds_object.c
    case_err_get_uint64_failed_array.c
    case_err_get_uint64_failed_object.c
    safuJsonGetUint32_utest.c
    safuJsonGetUint64_mock.c
  LIBRARIES
    safu_weak
    cmocka_mocks::mock_jsonc
)
