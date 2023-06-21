# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventbuffer_elosEventBufferDeleteMembers_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosEventBufferDeleteMembers_utest.c
  LIBRARIES
    eventbuffer_static
)
