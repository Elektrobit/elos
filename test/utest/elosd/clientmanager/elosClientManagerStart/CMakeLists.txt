# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerStart_utest
  SOURCES
    case_success.c
    case_exterr_plugin_manager_load.c
    case_err_clientmanager_null.c
    createMockConfigUtil.c
    elosClientManagerStart_utest.c
  LIBRARIES
    config_static
    clientmanager_static
    pluginmanager_static_weak
    mock_pluginmanager_static
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf
)
