# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosConnectSessionTcpip_utest
  SOURCES
    case_exterr_connect.c
    case_exterr_getaddrinfo.c
    case_exterr_socket.c
    case_success.c
    elosConnectSessionTcpip_utest.c
  LIBRARIES
    elos_event_static_weak
    safu::mock_safu
    cmocka_mocks::mock_libc
    libelos_static_weak
)
