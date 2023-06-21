// SPDX-License-Identifier: MIT
#ifndef SAFU_RINGBUFFER_RINGBUFFERNEW_H__
#define SAFU_RINGBUFFER_RINGBUFFERNEW_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/ringbuffer.h"

typedef struct safuUnitTestState {
    safuRingBuffer_t *ringBuffer;
} safuUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferNewErrParam)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferNewSuccess)

void safuTestCaseNew(void **state);
void safuTestCaseNew(void **state);

#endif /* SAFU_RINGBUFFER_RINGBUFFERNEW_H__ */
