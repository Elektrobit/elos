# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventbuffer_elosEventBufferInitialize_utest
  SOURCES
    case_err_func.c
    case_err_param.c
    case_success.c
    elosEventBufferInitialize_utest.c
  LIBRARIES
    eventbuffer_static
    safu::mock_safu
)
