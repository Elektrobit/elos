# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerThreadGetFreeConnectionSlot_utest
  SOURCES
    case_exterr_clock_gettime.c
    case_exterr_sem_timedwait.c
    case_exterr_mutex_lock.c
    case_exterr_mutex_unlock01.c
    case_exterr_mutex_unlock02.c
    case_recalculate_tv_sec.c
    case_errno_etimedout.c
    case_errno_eintr.c
    case_thread_not_joined.c
    case_no_free_connection.c
    case_success_first_connection.c
    case_success_middle_connection.c
    case_success_last_connection.c
    elosClientManagerThreadGetFreeConnectionSlot_utest.c
  LIBRARIES
    clientmanager_static_weak
    cmocka_mocks::mock_libc
)
