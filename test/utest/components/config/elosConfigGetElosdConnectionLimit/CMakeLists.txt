# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdConnectionLimit_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    case_err_invalid_limit_in_default.c
    case_err_invalid_limit_in_env.c
    case_err_non_int_limit_in_env.c
    elosConfigGetElosdConnectionLimit_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
)
