# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosEventSubscribe_utest
  SOURCES
    case_err_empty_list.c
    case_err_invalid_parameter.c
    case_err_response_has_no_ids_field.c
    case_exterr_communication_failed.c
    case_exterr_jsonAddNewArray_failed.c
    case_exterr_jsonAddNewString_failed.c
    case_exterr_jsonGetUint32_failed.c
    case_exterr_json_object_new_object_failed.c
    case_success.c
    elosEventSubscribe_utest.c
  LIBRARIES
    mock_libelos_static
    safu::mock_safu
    cmocka_mocks::mock_jsonc
)
