# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosConnectTcpip_utest
  SOURCES
    case_err_param.c
    case_exterr_allocmem.c
    case_success.c
    elosConnectTcpip_utest.c
  LIBRARIES
    elos_event_static_weak
    safu::mock_safu
    cmocka_mocks::mock_libc
    mock_libelos_static
)
