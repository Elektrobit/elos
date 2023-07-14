# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
  test_event_elosEventDeserialize_utest
  SOURCES
    case_err_empty_input_string.c
    case_err_empty_event.c
    case_exterr_event_conversion.c
    case_exterr_json_parse.c
    case_success_null_event_source.c
    case_success_null_event.c
    case_success_only_severity.c
    case_success_only_source.c
    elosEventDeserialize_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
)
