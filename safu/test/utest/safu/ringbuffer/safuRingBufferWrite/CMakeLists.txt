# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_ringbuffer_safuRingBufferWrite_utest
  SOURCES
    case_err_param.c
    case_success_threaded.c
    case_success.c
    safuRingBufferWrite_utest.c
  LIBRARIES
    safu_static
)
