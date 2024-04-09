# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
  test_scannerManager_elosScannerManagerInitialize_utest
  SOURCES
    case_success.c
    case_success_default_search_path_null.c
    createMockConfigUtil.c
    elosScannerManagerInitialize_utest.c
  LIBRARIES
    config_static
    scannermanager_static
    safu::mock_safu
    samconf::mock_samconf
)
