# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_logline_mapper_elosKmsgMapperDoMapping_utest
  SOURCES
    case_success.c
    case_err_empty_msg.c
    case_err_missing_number.c
    elosKmsgMapperDoMapping_utest.c
  LIBRARIES
    elos_event_static
    scanner_kmsg
)
