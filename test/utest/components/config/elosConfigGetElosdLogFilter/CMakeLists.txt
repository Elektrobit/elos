# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdLogFilter_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    elosConfigGetElosdLogFilter_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
)
