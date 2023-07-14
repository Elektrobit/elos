# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu)

create_unit_test(
  NAME
    test_pluginFilterLoader_elosPluginFilterLoaderLoad_utest
  SOURCES
    case_err_invalid_parameter.c
    case_err_no_filters_defined.c
    case_err_no_filter_section_defined.c
    case_success.c
    elosPluginFilterLoaderLoad_utest.c
  LIBRARIES
    eventlogging_static_weak
)
