// SPDX-License-Identifier: MIT
#ifndef SAFU_RINGBUFFER_RINGBUFFERDELETE_H__
#define SAFU_RINGBUFFER_RINGBUFFERDELETE_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/ringbuffer.h"

typedef struct safuUnitTestState {
    safuRingBuffer_t *ringBuffer;
    size_t value;
} safuUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferDeleteErrParam)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferDeleteSuccess)

void safuTestCaseNew(void **state);
void safuTestCaseDelete(void **state);

#endif /* SAFU_RINGBUFFER_RINGBUFFERDELETE_H__ */
