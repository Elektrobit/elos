# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.62.0 REQUIRED)
find_package(samconf 0.73.1 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigLoad_utest
  SOURCES
    case_success_load_default_config.c
    case_success_load_default_dir.c
    case_err_config_path.c
    case_err_load_and_merge.c
    case_err_load_and_merge_env.c
    elosConfigLoad_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
    cmocka_mocks::mock_libc
)
