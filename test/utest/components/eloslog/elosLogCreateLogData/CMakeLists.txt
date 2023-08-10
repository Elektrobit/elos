# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_unit_test(
  NAME
  test_eloslog_elosLogCreateLogData_utest
  SOURCES
    case_success.c
    elosLogCreateLogData_utest.c
  LIBRARIES
    eloslog_static_weak
    safu::mock_safu
    samconf::mock_samconf
)
