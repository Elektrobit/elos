# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_plugin_elosPluginControlUnsubscribe_utest
  SOURCES
    case_success.c
    case_err_invalid_parameter.c
    elosPluginControlUnsubscribe_utest.c
  LIBRARIES
    plugincontrol_static
    mock_eventprocessor_static
)
