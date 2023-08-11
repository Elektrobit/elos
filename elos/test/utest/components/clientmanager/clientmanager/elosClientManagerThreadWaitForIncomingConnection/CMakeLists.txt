# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerThreadWaitForIncomingConnection_utest
  SOURCES
    case_err_get_status.c
    case_listen_inactive.c
    case_err_pselect.c
    case_err_accept.c
    case_interrupt.c
    case_success_fd_ready.c
    case_success_fd_zero.c
    elosClientManagerThreadWaitForIncomingConnection_utest.c
  LIBRARIES
    clientmanager_static_weak
    mock_clientmanager_static
    cmocka_mocks::mock_libc
)
