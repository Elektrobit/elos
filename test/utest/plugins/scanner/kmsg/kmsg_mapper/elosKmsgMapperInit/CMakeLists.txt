# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_logline_mapper_elosKmsgMapperInit_utest
  SOURCES
    case_success.c
    case_err_open_proc.c
    case_err_fscanf_fail.c
    elosKmsgMapperInit_utest.c
  LIBRARIES
    scanner_kmsg_static
    scanner_manager_static
    cmocka_mocks::mock_libc
)
