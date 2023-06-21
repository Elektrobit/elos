# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeVectorRemoveById_utest
  SOURCES
    case_success.c
    case_vector_null.c
    case_invalid_id.c
    case_find_failed.c
    case_remove_failed.c
    elosEventFilterNodeVectorRemoveById_utest.c
  INCLUDES
    $<TARGET_PROPERTY:eventprocessor_static,INCLUDE_DIRECTORIES>
  LIBRARIES
    eventprocessor_static
    safu::mock_safu
)
