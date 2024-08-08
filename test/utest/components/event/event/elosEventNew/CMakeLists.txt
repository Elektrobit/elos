# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
  test_event_elosEventNew_utest
  SOURCES
    case_success.c
    case_exterr_allocMem.c
    elosEventNew_utest.c
  LIBRARIES
    elos_event_static
    safu::mock_safu
)
