# SPDX-License-Identifier: MIT
find_package(safu)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_libelos_elosEventVectorFromJsonObject_utest
  SOURCES
    case_err_event_vector_jobj.c
    case_err_event_vector.c
    case_exterr_get_array.c
    case_exterr_new_vector.c
    case_exterr_event_from_json_obj.c
    case_success.c

    elosEventVectorFromJsonObject_utest.c
  LIBRARIES
    elos_event_static_weak
    cmocka_mocks::mock_jsonc
    safu::mock_safu
    mock_elos_event_static
)
