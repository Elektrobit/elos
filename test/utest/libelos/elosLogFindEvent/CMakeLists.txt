# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosLogFindEvent_utest
  SOURCES
    case_err_session.c
    case_err_filter_rule.c
    case_err_vector.c
    case_exterr_verify_session.c
    case_exterr_new_object.c
    case_exterr_add_new_string.c
    case_exterr_comm_failed.c
    case_exterr_vector_from_json_arr.c
    case_success.c
    elosLogFindEvent_utest.c
  LIBRARIES
    mock_elos_event_static
    mock_libelos_static
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
