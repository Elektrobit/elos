# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
    test_logline_mapper_elosLoglineMapperDoMapping_utest
  SOURCES
    case_success.c
    elosLoglineMapperDoMapping_utest.c
  LIBRARIES
    scanner_syslog
    cmocka_mocks::mock_libc
    samconf::samconf_test_utils
    safu::mock_safu
)
