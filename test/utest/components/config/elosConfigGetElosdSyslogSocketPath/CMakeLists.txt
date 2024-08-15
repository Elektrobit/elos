# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdSyslogSocketPath_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    elosConfigGetElosdSyslogSocketPath_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
)
