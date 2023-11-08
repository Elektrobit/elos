# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_plugin_elosPluginControlDelete_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosPluginControlDelete_utest.c
  LIBRARIES
    plugincontrol_static
    cmocka_mocks::mock_libc
)
