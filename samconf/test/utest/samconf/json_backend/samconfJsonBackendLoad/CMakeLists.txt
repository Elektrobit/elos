# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks REQUIRED)
find_package(cmocka_extensions REQUIRED)
find_package(json-c REQUIRED)

create_unit_test(
  NAME
    test_samconfJsonBackendLoad_utest
  SOURCES
    case_success.c
    case_success_array_noloop.c
    case_error_compromised_config.c
    case_error_compromised_subconfig_object.c
    case_error_compromised_subconfig_array.c
    case_error_failed_add_object.c
    case_error_failed_add_array.c
    case_error_null_object_array.c
    case_error_null_object_object.c
    case_error_null_backend.c
    samconfJsonBackendLoad_utest.c
  LIBRARIES
    cmocka_extensions::cmocka_extensions
    samconf_weak
    mock_samconf_static
)
