# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventbuffer_elosEventBufferWrite_utest
  SOURCES
    case_err_func.c
    case_err_param.c
    case_success_threaded.c
    case_success.c
    elosEventBufferWrite_utest.c
  LIBRARIES
    eventbuffer_static
    safu::mock_safu
)
