# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_plugin_elosPluginLoad_utest
  SOURCES
    case_err_param.c
    case_success_funcoverride.c
    elosPluginLoad_functions.c
    elosPluginLoad_utest.c
  LIBRARIES
    plugin_static
    cmocka_mocks::mock_libc
)

# Symbols are not exported on normal executables;
# Exported symbols are needed for the unit tests here however,
# so we do set the necessary linker parameter here
target_link_options(
  test_plugin_elosPluginLoad_utest
  PRIVATE
    -Wl,--export-dynamic
)
