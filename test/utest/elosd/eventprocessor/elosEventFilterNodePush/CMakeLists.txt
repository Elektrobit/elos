# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_eventprocessor_elosEventFilterNodePush_utest
  SOURCES
    elosEventFilterNodePush_utest.c
    case_count_zero.c
    case_create_failed.c
    case_efe_null.c
    case_filter_null.c
    case_filter_string_null.c
    case_push_failed.c
    case_success.c
  LIBRARIES
    eventprocessor_static_weak
    mock_eventfilter_static
    mock_rpnfilter_static
    safu::mock_safu
)
