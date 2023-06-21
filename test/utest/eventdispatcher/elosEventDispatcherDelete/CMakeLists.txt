# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_eventdispatcher_elosEventDispatcherDelete_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosEventDispatcherDelete_utest.c
  LIBRARIES
    eventdispatcher_static_weak
    mock_eventdispatcher_static
)
