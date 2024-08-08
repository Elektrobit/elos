# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

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
