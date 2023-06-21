# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_vector_safuVecPush_utest
  SOURCES
    case_with_resize.c
    case_without_resize.c
    case_exterr_realloc.c
	safuVecPush_utest.c
  LIBRARIES
    safu_static
    cmocka_mocks::mock_libc
)
