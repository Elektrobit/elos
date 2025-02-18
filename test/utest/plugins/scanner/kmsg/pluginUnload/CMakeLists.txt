# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)
find_package(cmocka_mocks 0.55.3 REQUIRED)

create_unit_test(
  NAME
    test_kmsg_pluginUnload_utest
  SOURCES
    case_err_invalid_plugin.c
    case_success.c
    pluginUnload_utest.c
  LIBRARIES
    mock_libelosplugin_static
    mock_scanner_kmsg_static
    scanner_kmsg_static_weak
    cmocka_mocks::mock_libc
    safu::mock_safu
    samconf::mock_samconf
)