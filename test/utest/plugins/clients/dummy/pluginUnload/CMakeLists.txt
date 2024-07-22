# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_dummy_pluginUnload_utest
  SOURCES
    case_success.c
    case_err_invalid_plugin.c
    pluginUnload_utest.c
  LIBRARIES
    client_dummy
)
