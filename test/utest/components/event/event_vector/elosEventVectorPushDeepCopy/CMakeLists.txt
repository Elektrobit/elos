# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
  test_vector_elosEventVectorPushDeepCopy_utest
  SOURCES
    case_mock.c
    case_copy_failed.c
    elosEventVectorPushDeepCopy_utest.c
  LIBRARIES
    safu::mock_safu
    elos_event_static_weak
    mock_elos_event_static
)
