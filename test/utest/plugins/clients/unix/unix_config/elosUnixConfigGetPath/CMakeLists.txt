# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.53.3 REQUIRED)

create_unit_test(
  NAME
    test_config_elosUnixConfigGetPath_utest
  SOURCES
    case_success_from_config.c
    case_success_from_default.c
    elosUnixConfigGetPath_utest.c
  LIBRARIES
    client_unix
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf_test_utils
)
