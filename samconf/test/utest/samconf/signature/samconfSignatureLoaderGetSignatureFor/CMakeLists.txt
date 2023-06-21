# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_samconfSignatureLoaderGetSignatureFor_utest
  SOURCES
    case_success.c
    case_allocerror.c
    case_accesserror.c
    case_readerror.c
    samconfSignatureLoaderGetSignatureFor_utest.c
  LIBRARIES
    samconf_weak
    safu::mock_safu
    cmocka_mocks::mock_libc
)
