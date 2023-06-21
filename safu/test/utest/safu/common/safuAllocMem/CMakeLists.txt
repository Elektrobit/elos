# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_common_safuAllocMem_utest
  SOURCES
    case_success_new_mem.c
    case_success_oldptr_null.c
    case_success_newlen_zero.c
    case_success_all_param_zero.c
    case_exterr_realloc.c
    safuAllocMem_utest.c
  LIBRARIES
    mock_safu_static
    cmocka_mocks::mock_libc
)
