# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeVectorInitialize_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosEventFilterNodeVectorInitialize_utest.c
  INCLUDES
    $<TARGET_PROPERTY:eventprocessor_static,INCLUDE_DIRECTORIES>
  LIBRARIES
    eventprocessor_static
)
