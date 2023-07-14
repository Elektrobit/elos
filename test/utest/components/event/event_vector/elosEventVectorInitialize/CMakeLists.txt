# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_libelos_elosEventVectorInitialize_utest
  SOURCES
    case_err_event_vector.c
    case_success.c
    elosEventVectorInitialize_utest.c
  LIBRARIES
    elos_event_static
)
