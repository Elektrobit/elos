# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdScannerPath_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    elosConfigGetElosdScannerPath_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
)
