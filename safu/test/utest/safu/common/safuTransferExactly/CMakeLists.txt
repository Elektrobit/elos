# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuTransferExactly_utest
  SOURCES
    case_err_fd_lt_zero.c
    case_err_buff_null.c
    case_err_len_lt_one.c
    case_err_transfer_bytes_lt_zero.c
    case_err_transfer_bytes_eq_zero.c
    case_success_in_one_loop.c
    case_success_in_two_loops.c
    case_success_eintr.c
    mock_transferFunc.c
    safuTransferExactly_utest.c
  LIBRARIES
    safu_weak
)
