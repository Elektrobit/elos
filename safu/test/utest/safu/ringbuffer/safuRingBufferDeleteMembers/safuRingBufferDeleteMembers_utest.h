// SPDX-License-Identifier: MIT
#ifndef SAFU_RINGBUFFER_RINGBUFFERDELETEMEMBERS_H__
#define SAFU_RINGBUFFER_RINGBUFFERDELETEMEMBERS_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/ringbuffer.h"

typedef struct safuUnitTestState {
    safuRingBuffer_t ringBuffer;
    size_t *values;
} safuUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferDeleteMembersErrParam)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuRingBufferDeleteMembersSuccess)

void safuTestCaseNew(void **state);
void safuTestCaseDelete(void **state);

#endif /* SAFU_RINGBUFFER_RINGBUFFERDELETEMEMBERS_H__ */
