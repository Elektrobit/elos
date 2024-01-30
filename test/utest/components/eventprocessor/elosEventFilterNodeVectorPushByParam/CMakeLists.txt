# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodeVectorPushByParam_utest
  SOURCES
    case_initialize_failed.c
    case_param_null.c
    case_push_failed.c
    case_success.c
    case_vector_null.c
    elosEventFilterNodeVectorPushByParam_utest.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventprocessor_static
    safu::mock_safu
)
