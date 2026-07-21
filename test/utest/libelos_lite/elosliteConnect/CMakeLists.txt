# SPDX-License-Identifier: MIT

find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos-lite_eloslite_connect_unix_utest
  SOURCES
    utest_elosliteConnectUnix.c
    elosliteConnect_utest.c
  LIBRARIES
    libelos-lite
    libelos_common
    safu::safu
)

target_link_options(
    test_libelos-lite_eloslite_connect_unix_utest
    PUBLIC
    -Wl,--wrap=socket
    -Wl,--wrap=connect
)
