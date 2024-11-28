# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_scannerManager_elosScannerManagerStart_utest
  SOURCES
    case_success.c
    case_success_empty_plugins_list.c
    case_success_no_plugins_list.c
    case_exterr_plugin_manager_load.c
    case_err_scannermanager_null.c
    createMockConfigUtil.c
    elosScannerManagerStart_utest.c
  LIBRARIES
    config_static
    scannermanager_static
    pluginmanager_static_weak
    mock_pluginmanager_static
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf
)
