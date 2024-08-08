# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
  test_vector_elosEventVectorPush_utest
  SOURCES
    case_mock.c
    elosEventVectorPush_utest.c
  LIBRARIES
    safu::mock_safu
    elos_event_static
)
