# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_unix_pluginUnload_utest
  SOURCES
    case_err_invalid_plugin.c
    case_success.c
    pluginUnload_utest.c
  LIBRARIES
    client_unix
)
