# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.56.1 REQUIRED)

create_unit_test(
  NAME
    test_libelosdlt_elosDltMapperInit_utest
  SOURCES
    case_error.c
    case_success.c
    elosDltMapperInit_utest.c
  LIBRARIES
    libelosdlt
    safu::safu
)
