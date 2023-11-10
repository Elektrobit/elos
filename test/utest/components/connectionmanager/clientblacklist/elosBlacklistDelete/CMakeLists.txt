# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu REQUIRED)

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
    connectionmanager_static_weak
    mock_connectionmanager_static
    mock_eventfilter_static
    safu::mock_safu
)
