# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_plugin_elosPluginControlGetName_utest
  SOURCES
    case_success.c
    case_err_invalid_parameter.c
    elosPluginControlGetName_utest.c
  LIBRARIES
    plugincontrol_static
)
