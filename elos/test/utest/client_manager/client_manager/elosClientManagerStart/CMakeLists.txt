# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerStart_utest
  SOURCES
    case_err_context_null.c
    case_err_blacklist_initialize.c
    case_err_parameters_null.c
    case_exterr_inet_pton.c
    case_exterr_context_mutex_init.c
    case_exterr_sem_init.c
    case_exterr_connection_mutex_init_first.c
    case_exterr_connection_mutex_init_mid.c
    case_exterr_connection_mutex_init_last.c
    case_exterr_socket.c
    case_exterr_setsockopt.c
    case_exterr_bind.c
    case_exterr_listen.c
    case_exterr_pthread_create.c
    case_success.c
    elosClientManagerStart_utest.c
  LIBRARIES
    cmocka_mocks::mock_libc
    clientmanager_static_weak
    mock_client_manager_static
    mock_eventfilter_static
    safu::mock_safu
    samconf::mock_samconf
)
