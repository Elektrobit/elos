# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_unit_test(
  NAME
    test_config_elosConfigGetElosdBackendPath_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    elosConfigGetElosdBackendPath_utest.c
  LIBRARIES
    config_static
    safu::mock_safu
    samconf::mock_samconf
)
