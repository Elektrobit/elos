# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_libelos_elosEventVectorDeleteMembers_utest
  SOURCES
    case_err_event_vector.c
    case_success.c

    elosEventVectorDeleteMembers_utest.c
  LIBRARIES
    elos_event_static
)
