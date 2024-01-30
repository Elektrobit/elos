# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosEventVectorNew_utest
  SOURCES
    case_err_event_vector.c
    case_exterr_alloc_mem.c
    case_success.c
    elosEventVectorNew_utest.c
  LIBRARIES
    elos_event_static
    safu::mock_safu
)
