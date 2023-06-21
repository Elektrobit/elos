# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_ringbuffer_safuRingBufferDelete_utest
  SOURCES
    case_err_param.c
    case_success.c
    safuRingBufferDelete_utest.c
  LIBRARIES
    safu_static
)
