# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_plugin_elosPluginGetName_utest
  SOURCES
    case_success.c
    case_err_invalid_parameter.c
    elosPluginGetName_utest.c
  LIBRARIES
    plugin_static
)
