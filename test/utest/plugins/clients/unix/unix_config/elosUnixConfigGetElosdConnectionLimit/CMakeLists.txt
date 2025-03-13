# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.53.3 REQUIRED)

create_unit_test(
  NAME
    test_config_elosUnixConfigGetElosdConnectionLimit_utest
  SOURCES
    case_success_from_env.c
    case_success_from_default.c
    case_err_non_int_limit_in_env.c
    elosUnixConfigGetElosdConnectionLimit_utest.c
  INCLUDES
    "${CMAKE_SOURCE_DIR}/src/plugins/clients/Unix/private"
  LIBRARIES
    libelosplugin_interface
    client_unix
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf_test_utils
)
