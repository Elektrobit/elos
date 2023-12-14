# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerStop_utest
  SOURCES
    case_success.c
    case_exterr_plugin_manager_unload.c
    case_err_clientmanager_null.c
    case_err_param_pluginmanager_null.c
    createMockConfigUtil.c
    elosClientManagerStop_utest.c
  LIBRARIES
    config_static
    clientmanager_static
    pluginmanager_static_weak
    mock_pluginmanager_static
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf
)
