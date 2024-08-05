# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
  test_event_elosEventSerialize_utest
  SOURCES
    case_err_null_event.c
    case_err_null_storage_string.c
    case_exterr_new_json_obj.c
    case_exterr_event_conversion_error.c
    case_exterr_json_obj_string_error.c
    case_exterr_strdup.c
    case_success_empty_event.c
    case_success_null_event_source.c
    case_success_only_severity.c
    case_success_only_source.c
    case_success_only_time.c
    elosEventSerialize_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_jsonc
)
