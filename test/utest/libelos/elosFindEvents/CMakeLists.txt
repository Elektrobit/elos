# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosFindEvents_utest
  SOURCES
    case_err_invalid_parameter.c
    case_exterr_verify_session.c
    case_exterr_new_object.c
    case_exterr_add_new_string.c
    case_exterr_add_new_timestamp.c
    case_exterr_comm_failed.c
    case_exterr_vector_from_json_arr.c
    case_success.c
    case_success_truncated.c
    elosFindEvents_utest.c
  LIBRARIES
    mock_elos_event_static
    mock_libelos_static
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
