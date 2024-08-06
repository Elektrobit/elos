# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdLogLevel_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    case_success_from_config.c
    case_err_env_fallback_to_config.c
    case_err_fallback_to_default.c
    elosConfigGetElosdLogLevel_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
    cmocka_mocks::mock_libc
)
