# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuRecvExactly_utest
  SOURCES
    mock_transferExactly.c
    case_recv_exactly.c
    safuRecvExactly_utest.c
  LIBRARIES
    safu_weak
)
