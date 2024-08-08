# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_scannerManager_elosScannerManagerInitialize_utest
  SOURCES
    case_success.c
    case_success_default_search_path_null.c
    case_exterr_scanner_config_null.c
    case_exterr_plugin_vector_create.c
    case_err_scannermanager_null.c
    case_err_param_config_null.c
    case_err_param_pluginmanager_null.c
    case_err_init_param_null.c
    createMockConfigUtil.c
    elosScannerManagerInitialize_utest.c
  LIBRARIES
    config_static
    scannermanager_static
    safu::mock_safu
    samconf::mock_samconf
)
