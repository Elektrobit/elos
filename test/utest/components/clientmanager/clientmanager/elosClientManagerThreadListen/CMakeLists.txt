# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerThreadListen_utest
  SOURCES
    case_err_get_status.c
    case_listen_inactive.c
    case_err_get_free_slot.c
    case_slot_lt_zero.c
    case_err_mutex_lock.c
    case_err_wait_for_connection.c
    case_err_mutex_unlock01.c
    case_err_sem_unlock01.c
    case_err_pthread_create.c
    case_err_sem_unlock02.c
    case_err_mutex_unlock02.c
    case_success_ones.c
    case_success_all.c
    elosClientManagerThreadListen_utest.c
  LIBRARIES
    clientmanager_static_weak
    mock_clientmanager_static
    cmocka_mocks::mock_libc
)
