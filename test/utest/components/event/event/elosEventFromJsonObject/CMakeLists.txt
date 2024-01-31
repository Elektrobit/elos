# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
  test_event_elosEventFromJsonObject_utest
  SOURCES
    case_err_null_input_json_object.c
    case_err_null_input_event.c
    case_exterr_get_classification_error.c
    case_exterr_get_date_error.c
    case_exterr_get_hardwareid_error.c
    case_exterr_get_messageCode_error.c
    case_exterr_get_payload_error.c
    case_exterr_get_severity_error.c
    case_exterr_get_source_error.c
    case_success_empty_jobject.c
    case_success_null_event_source.c
    case_success_only_severity.c
    case_success_only_source.c
    elosEventFromJsonObject_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
