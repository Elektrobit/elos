# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerInitialize_utest
  SOURCES
    case_success.c
    case_success_default_search_path_null.c
    case_exterr_client_config_null.c
    case_exterr_plugin_vector_create.c
    case_err_clientmanager_null.c
    case_err_param_config_null.c
    case_err_param_pluginmanager_null.c
    case_err_init_param_null.c
    createMockConfigUtil.c
    elosClientManagerInitialize_utest.c
  LIBRARIES
    config_static
    clientmanager_static_weak
    safu::mock_safu
    samconf::mock_samconf
)
