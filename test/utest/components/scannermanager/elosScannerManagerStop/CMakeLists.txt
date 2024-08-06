# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
  test_scannerManager_elosScannerManagerStop_utest
  SOURCES
    case_success.c
    case_exterr_plugin_manager_unload.c
    case_err_scannermanager_null.c
    case_err_param_pluginmanager_null.c
    createMockConfigUtil.c
    elosScannerManagerStop_utest.c
  LIBRARIES
    config_static
    scannermanager_static
    pluginmanager_static_weak
    mock_pluginmanager_static
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf
)
