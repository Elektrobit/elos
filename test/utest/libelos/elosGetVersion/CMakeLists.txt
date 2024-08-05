# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosGetVersion_utest
  SOURCES
    case_err_call.c
    case_err_param.c
    case_success.c
    elosGetVersion_utest.c
  LIBRARIES
    scanner_manager_legacy_static_weak
    mock_libelos_static
    libelos_static_weak
    safu::mock_safu
    cmocka_mocks::mock_jsonc
)
