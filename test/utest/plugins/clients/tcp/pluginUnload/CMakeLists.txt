# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_tcp_pluginUnload_utest
  SOURCES
    case_err_invalid_plugin.c
    case_success.c
    pluginUnload_utest.c
  LIBRARIES
    client_tcp
)
