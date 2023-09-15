# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_plugin_elosPluginDelete_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosPluginDelete_utest.c
  LIBRARIES
    plugin_static
    cmocka_mocks::mock_libc
)
