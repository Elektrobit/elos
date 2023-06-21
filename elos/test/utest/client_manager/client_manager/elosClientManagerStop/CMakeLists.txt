# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerStop_utest
  SOURCES
    case_err_context_null.c
    case_exterr_sem_destroy.c
    case_success_close_connections.c
    case_success_no_clientmanager.c
    case_success_clientmanager_started.c
    elosClientManagerStop_utest.c
  LIBRARIES
    clientmanager_static_weak
    mock_client_manager_static
    cmocka_mocks::mock_libc
)
