# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_storageManager_elosStorageManagerDeleteMembers_utest
  SOURCES
    case_success.c
    case_exterr_plugin_vector_free.c
    case_err_storagemanager_null.c
    case_err_param_pluginmanager_null.c
    createMockConfigUtil.c
    elosStorageManagerDeleteMembers_utest.c
  LIBRARIES
    config_static
    storagemanager_static_weak
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf
)
