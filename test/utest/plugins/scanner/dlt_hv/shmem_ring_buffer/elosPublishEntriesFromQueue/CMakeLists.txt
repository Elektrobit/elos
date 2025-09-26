# SPDX-License-Identifier: MIT
find_package(samconf 0.53.3 REQUIRED)

create_unit_test(
  NAME
    test_shmem_ring_buffer_elosPublishEntriesFromQueue_utest
  SOURCES
    case_success.c
	 elosPublishEntriesFromQueue_utest.c
  LIBRARIES
    samconf::samconf_test_utils
    elos_event_static
    scanner_dlt_hv
)
