# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.55.7 REQUIRED)

create_unit_test(
  NAME
    test_config_elosUnixConfigGetSocketAddress_utest
  SOURCES
    case_err_no_access.c
    case_success_create_dir.c
    case_success_path_exist.c
    elosUnixConfigGetSocketAddress_utest.c
  LIBRARIES
    mock_unix_config_static
    unix_config_static_weak
    cmocka_mocks::mock_libc
)
