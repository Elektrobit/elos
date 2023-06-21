// SPDX-License-Identifier: MIT
#ifndef SAFU_RINGBUFFER_RINGBUFFERREAD_H__
#define SAFU_RINGBUFFER_RINGBUFFERREAD_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/ringbuffer.h"

typedef struct safuUnitTestState {
    safuRingBuffer_t ringBuffer;
    safuVec_t vector;
    size_t *values;
} safuUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferReadErrParam)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferReadSuccess)

void safuTestCaseNew(void **state);
void safuTestCaseDelete(void **state);

#endif /* SAFU_RINGBUFFER_RINGBUFFERREAD_H__ */
