# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.53.3 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdPort_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    elosConfigGetElosdPort_utest.c
  LIBRARIES
    client_tcp
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf_test_utils
)
