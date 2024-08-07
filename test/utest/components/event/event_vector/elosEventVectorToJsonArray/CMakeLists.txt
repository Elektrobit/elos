# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosEventVectorToJsonArray_utest
  SOURCES
    case_err_array_add.c
    case_err_array_new.c
    case_err_param.c
    case_success.c
    elosEventVectorToJsonArray_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
    cmocka_mocks::mock_jsonc
    safu::mock_safu
)
