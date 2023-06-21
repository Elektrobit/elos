# SPDX-License-Identifier: MIT
find_package(safu)
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_libelos_elosConnectTcpip_utest
  SOURCES
    case_err_param.c
    case_exterr_allocmem.c
    case_exterr_inet_aton.c
    case_exterr_socket.c
    case_exterr_connect.c
    case_success.c
    elosConnectTcpip_utest.c
  LIBRARIES
    scanner_manager_static_weak
    elos_event_static_weak
    libelos_static_weak
    safu::mock_safu
    cmocka_mocks::mock_libc
)
