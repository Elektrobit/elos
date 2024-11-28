# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    test_StorageBackend_elosStorageBackendAccepts_utest
  SOURCES
    case_err_invalid_parameter.c
    case_success.c
    elosStorageBackendAccepts_utest.c
  LIBRARIES
    eventlogging_static_weak
)
