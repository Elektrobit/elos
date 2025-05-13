# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdLogLevel_utest
  SOURCES
    case_success_from_default.c
    case_success_from_config.c
    elosConfigGetElosdLogLevel_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf_test_utils
    cmocka_mocks::mock_libc
)
