# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

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
