# SPDX-License-Identifier: MIT
find_package(safu)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_libelos_elosEventVectorFromJsonArray_utest
  SOURCES
    case_err_array.c
    case_err_vector.c
    case_err_param.c
    case_success.c
    elosEventVectorFromJsonArray_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
