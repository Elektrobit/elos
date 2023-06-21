# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
    test_common_safuTimeGetLocalTime_utest
  SOURCES
    case_time_return_valid_value.c
    safuTimeGetLocalTime_utest.c
  LIBRARIES
    safu_weak
    cmocka_mocks::mock_libc
)
