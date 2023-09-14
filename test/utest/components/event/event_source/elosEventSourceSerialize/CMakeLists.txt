# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_eventSource_elosEventSourceSerialize_utest
  SOURCES
    case_err_null.c
    case_exterr_eventSourceToJsonObject.c
    case_exterr_json_object_string.c
    case_exterr_strdup.c
    case_success_complete.c
    case_success_empty.c
    case_success_wo_appName.c
    case_success_wo_fileName.c
    case_success_wo_pid.c
    elosEventSourceSerialize_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
    cmocka_mocks::mock_jsonc
    cmocka_mocks::mock_libc
)
