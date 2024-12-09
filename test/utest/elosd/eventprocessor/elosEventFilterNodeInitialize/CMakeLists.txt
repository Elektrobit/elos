# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeInitialize_utest
  SOURCES
    case_err_double_init.c
    case_err_mutex.c
    case_err_param.c
    case_success.c
    elosEventFilterNodeInitialize_utest.c
  LIBRARIES
    eventprocessor_static
    cmocka_mocks::mock_libc
)
