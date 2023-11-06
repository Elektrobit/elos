# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu)

create_unit_test(
  NAME
  test_logAggregator_elosLogAggregatorFindEvents_utest
  SOURCES
    case_err_param_logAggregator_null.c
    case_err_param_rule_null.c
    case_err_param_events_null.c
    case_exterr_eventFilterCreate.c
    case_exterr_mutex.c
    elosLogAggregatorFindEvents_utest.c
  LIBRARIES
    plugincontrol_static
    backend_json_static
    eventlogging_static_weak
    cmocka_mocks::mock_libc
    safu::mock_safu
    mock_eventfilter_static
)
