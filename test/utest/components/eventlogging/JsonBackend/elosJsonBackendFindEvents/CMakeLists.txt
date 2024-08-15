# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
  test_jsonBackend_elosJsonBackendFindEvents_utest
  SOURCES
    case_success.c
    elosJsonBackendFindEvents_utest.c
  LIBRARIES
    backend_json_static
    cmocka_mocks::mock_libc
    safu::mock_safu
)
