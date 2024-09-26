# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_event_elosEventDeepCopy_utest
  SOURCES
    case_exterr_strdup_hardwareid_error.c
    case_exterr_strdup_payload_error.c
    case_exterr_event_source_deep_copy.c
    case_err_null_input.c
    case_success_event_source_not_null.c
    case_success_null_event_source.c
    case_success_null_event_member_strings.c
    elosEventDeepCopy_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
    cmocka_mocks::mock_libc
)
