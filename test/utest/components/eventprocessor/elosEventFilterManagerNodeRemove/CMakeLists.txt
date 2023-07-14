# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterManagerNodeRemove_utest
  SOURCES
    case_err_param.c
    case_err_remove.c
    case_success.c
    elosEventFilterManagerNodeRemove_utest.c
  INCLUDES
    $<TARGET_PROPERTY:eventprocessor_static,INCLUDE_DIRECTORIES>
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    safu::mock_safu
)
