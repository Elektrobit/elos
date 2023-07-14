# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_clientManager_elosClientManagerGetStatus_utest
  SOURCES
    case_exterr_mutex_lock.c
    case_exterr_mutex_unlock.c
    case_success.c
    elosClientManagerGetStatus_utest.c
  LIBRARIES
    scanner_manager_static_weak
    cmocka_mocks::mock_libc
)
