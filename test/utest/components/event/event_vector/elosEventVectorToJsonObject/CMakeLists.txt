# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosEventVectorToJsonObject_utest
  SOURCES
    case_err_event_vector.c
    case_err_event_vector_jobj.c
    case_exterr_new_object.c
    case_exterr_add_new_array.c
    case_success.c

    elosEventVectorToJsonObject_utest.c
  LIBRARIES
    elos_event_static
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
