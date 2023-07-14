# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeVectorDeleteMembers_utest
  SOURCES
    case_null_param.c
    case_null_data.c
    case_free_failed.c
    case_iterate_failed.c
    case_success.c
    elosEventFilterNodeVectorDeleteMembers_utest.c
  INCLUDES
    $<TARGET_PROPERTY:eventprocessor_static,INCLUDE_DIRECTORIES>
  LIBRARIES
    eventprocessor_static
    safu::mock_safu
)
