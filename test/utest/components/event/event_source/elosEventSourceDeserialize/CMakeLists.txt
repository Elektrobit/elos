# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_eventSource_elosEventSourceDeserialize_utest
  SOURCES
    case_err_null.c
    case_exterr_json_tokener.c
    case_success_complete.c
    case_success_empty.c
    case_success_wo_appName.c
    case_success_wo_fileName.c
    case_success_wo_pid.c
    elosEventSourceDeserialize_utest.c
  LIBRARIES
    elos_event_static
)
