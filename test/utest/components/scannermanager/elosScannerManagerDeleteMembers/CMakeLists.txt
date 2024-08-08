# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
  test_scannerManager_elosScannerManagerDeleteMembers_utest
  SOURCES
    case_success.c
    case_exterr_plugin_vector_free.c
    case_err_scannermanager_null.c
    case_err_param_pluginmanager_null.c
    createMockConfigUtil.c
    elosScannerManagerDeleteMembers_utest.c
  LIBRARIES
    config_static
    scannermanager_static
    safu::mock_safu
    samconf::samconf
)
