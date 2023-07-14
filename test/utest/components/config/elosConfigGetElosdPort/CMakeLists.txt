# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdPort_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    elosConfigGetElosdPort_utest.c
  LIBRARIES
    scanner_manager_static
    safu::mock_safu
    samconf::mock_samconf
)
