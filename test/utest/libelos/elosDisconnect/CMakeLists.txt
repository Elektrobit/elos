# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosDisconnect_utest
  SOURCES
    case_exterr_close.c
    case_success.c
    elosDisconnect_utest.c
  LIBRARIES
    elos_event_static_weak
    safu::mock_safu
    cmocka_mocks::mock_libc
    mock_libelos_static
)
