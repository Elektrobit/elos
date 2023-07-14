# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_unit_test(
  NAME
  test_clientManager_elosBlacklistInitialize_utest
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
    clientmanager_static_weak
    mock_client_manager_static
    mock_eventfilter_static
    safu::mock_safu
    samconf::mock_samconf
)
