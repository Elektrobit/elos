# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
  test_connectionManager_elosBlacklistDelete_utest
  SOURCES
    case_success.c
    case_err_blacklist_param_null.c
    case_exterr_filter_delete.c
    elosBlacklistDelete_utest.c
  LIBRARIES
    cmocka_mocks::mock_libc
    libelosplugin_interface
    mock_connectionmanager_static
    mock_eventfilter_static
    safu::mock_safu
)
