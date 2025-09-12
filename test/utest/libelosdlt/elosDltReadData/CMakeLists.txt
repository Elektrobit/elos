# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelosdlt_elosDltReadData_utest
  SOURCES
    case_success.c
    case_err_invalid_msg.c
    case_err_incomplete_msg.c
    case_err_incomplete_hdr.c
    elosDltReadData_utest.c
  LIBRARIES
    libelosdlt
    safu::safu
)
