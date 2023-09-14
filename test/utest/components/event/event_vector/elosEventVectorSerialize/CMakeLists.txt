# SPDX-License-Identifier: MIT
find_package(safu)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_libelos_elosEventVectorSerialize_utest
  SOURCES
    case_err_conversion.c
    case_err_param.c
    case_success.c
    elosEventVectorSerialize_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
    cmocka_mocks::mock_jsonc
    cmocka_mocks::mock_libc
    safu::mock_safu
)
