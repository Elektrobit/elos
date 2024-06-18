# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(samconf 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_logline_mapper_elosLoglineMapperInit_utest
  SOURCES
    case_success.c
    case_sectioned_message_codes.c
    case_err_deep_nested_message_codes.c
    case_config_null.c
    elosLoglineMapperInit_utest.c
  LIBRARIES
    scanner_syslog
    cmocka_mocks::mock_libc
    samconf::samconf_test_utils
    samconf::samconf
)
