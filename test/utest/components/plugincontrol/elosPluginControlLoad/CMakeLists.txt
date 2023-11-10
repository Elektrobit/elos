# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_plugin_elosPluginControlLoad_utest
  SOURCES
    case_err_param.c
    case_success_local.c
    elosPluginControlLoad_functions.c
    elosPluginControlLoad_utest.c
  LIBRARIES
    plugincontrol_static
    cmocka_mocks::mock_libc
)

# Symbols are not exported on normal executables;
# Exported symbols are needed for the unit tests here however,
# so we do set the necessary linker parameter here
target_link_options(
  test_plugin_elosPluginControlLoad_utest
  PRIVATE
    -Wl,--export-dynamic
)
