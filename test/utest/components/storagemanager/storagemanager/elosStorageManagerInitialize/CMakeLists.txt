# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_storageManager_elosStorageManagerInitialize_utest
  SOURCES
    case_err_init_param_null.c
    case_err_param_config_null.c
    case_err_param_pluginmanager_null.c
    case_err_storagemanager_null.c
    case_success_backend_config_null.c
    case_exterr_plugin_vector_create.c
    case_success.c
    case_success_default_search_path.c
    createMockConfigUtil.c
    elosStorageManagerInitialize_utest.c
  LIBRARIES
    config_static
    storagemanager_static_weak
    safu::mock_safu
    samconf::mock_samconf
)
