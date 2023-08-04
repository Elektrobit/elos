# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerStop_utest
  SOURCES
    case_err_context_null.c
    case_exterr_sem_destroy.c
    case_success_no_clientmanager.c
    elosClientManagerStop_utest.c
  LIBRARIES
    clientmanager_static_weak
    mock_client_manager_static
    mock_eventdispatcher_static
    cmocka_mocks::mock_libc
)
