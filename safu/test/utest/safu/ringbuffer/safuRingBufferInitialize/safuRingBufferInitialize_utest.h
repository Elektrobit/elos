// SPDX-License-Identifier: MIT
#ifndef SAFU_RINGBUFFER_RINGBUFFERINITIALIZE_H__
#define SAFU_RINGBUFFER_RINGBUFFERINITIALIZE_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/ringbuffer.h"

typedef struct safuUnitTestState {
    safuRingBuffer_t ringBuffer;
    size_t *values;
} safuUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferInitializeSuccess)

void safuTestCaseNew(void **state);
void safuTestCaseDelete(void **state);

#endif /* SAFU_RINGBUFFER_RINGBUFFERINITIALIZE_H__ */
