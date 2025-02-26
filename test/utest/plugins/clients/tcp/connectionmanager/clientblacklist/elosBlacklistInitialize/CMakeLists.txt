# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_connectionManager_elosBlacklistInitialize_utest
  SOURCES
    case_success.c
    case_exterr_filter_string.c
    case_exterr_filter_create.c
    case_exterr_config_get_string.c
    case_err_blacklist_param_null.c
    case_err_config_param_null.c
    elosBlacklistInitialize_utest.c
  LIBRARIES
    cmocka_mocks::mock_libc
    libelosplugin_interface
    mock_connectionmanager_static
    mock_eventfilter_static
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf_test_utils
)
