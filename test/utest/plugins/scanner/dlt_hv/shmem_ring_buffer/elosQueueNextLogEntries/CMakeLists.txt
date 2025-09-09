# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_shmem_ring_buffer_elosQueueNextLogEntries_utest
  SOURCES
    case_success.c
    case_success_empty.c
    case_err_idx_to_big.c
    elosQueueNextLogEntries_utest.c
  LIBRARIES
    elos_event_static
    scanner_dlt_hv
)
