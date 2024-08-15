# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
  test_event_elosEventToJsonObject_utest
  SOURCES
    case_err_event_null.c
    case_err_jobj_null.c
    case_exterr_get_classification_error.c
    case_exterr_get_date_error.c
    case_exterr_get_hardwareid_error.c
    case_exterr_get_messageCode_error.c
    case_exterr_get_payload_error.c
    case_exterr_get_severity_error.c
    case_exterr_get_source_add_object_error.c
    case_exterr_get_source_error.c
    case_exterr_json_object.c
    case_success_null_event_source.c
    case_success_only_severity.c
    case_success_valid_time_values.c
    case_success_zero_initialized.c
    elosEventToJsonObject_utest.c
  LIBRARIES
    elos_event_static_weak
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_jsonc
    safu::mock_safu
    mock_elos_event_static
    cmocka_extensions::cmocka_extensions
)
