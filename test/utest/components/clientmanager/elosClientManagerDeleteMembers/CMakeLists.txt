# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)
find_package(safu 0.50.1 REQUIRED)
find_package(samconf REQUIRED)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerDeleteMembers_utest
  SOURCES
    case_success.c
    case_exterr_plugin_vector_free.c
    case_err_clientmanager_null.c
    case_err_param_pluginmanager_null.c
    createMockConfigUtil.c
    elosClientManagerDeleteMembers_utest.c
  LIBRARIES
    config_static
    clientmanager_static_weak
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf
)
