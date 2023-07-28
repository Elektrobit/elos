# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
    test_eventdispatcher_elosEventDispatcherWorker_tsan
  SOURCES
    case_success.c
    elosEventDispatcherWorker_tsan.c
  LIBRARIES
    eventdispatcher_static_weak
    safu::safu
)
