# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_plugin_elosPluginInitialize_utest
  SOURCES
    case_err_func.c
    case_err_param.c
    case_success_funcoverride.c
    case_success.c
    elosPluginInitialize_utest.c
  LIBRARIES
    plugin_static
    cmocka_mocks::mock_libc
)
