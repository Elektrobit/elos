# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_storageManager_elosStorageManagerStop_utest
  SOURCES
    case_success.c
    case_exterr_plugin_manager_unload.c
    case_err_storagemanager_null.c
    case_err_param_pluginmanager_null.c
    createMockConfigUtil.c
    elosStorageManagerStop_utest.c
  LIBRARIES
    config_static
    storagemanager_static
    pluginmanager_static_weak
    mock_pluginmanager_static
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf
)
