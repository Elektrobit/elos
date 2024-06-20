# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

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
