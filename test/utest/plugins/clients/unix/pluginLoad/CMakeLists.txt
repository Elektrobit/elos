# SPDX-License-Identifier: MIT

find_package(samconf 0.53.3 REQUIRED)

create_unit_test(
  NAME
    test_unix_pluginLoad_utest
  SOURCES
    case_err_invalid_plugin.c
    case_success.c
    pluginLoad_utest.c
  LIBRARIES
    client_unix
    samconf::samconf_test_utils
)
