# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuSendExactly_utest
  SOURCES
    mock_transferExactly.c
    case_send_exactly.c
    safuSendExactly_utest.c
  LIBRARIES
    safu_weak
)
