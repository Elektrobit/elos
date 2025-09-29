// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>

#include "dlt_hv/shmem_ring_buffer.h"
#include "elosQueueNextLogEntries_utest.h"

int elosTestElosQueueNextLogEntriesSuccessEmptySetup(void **state) {
    struct elosTestData *testData = *state;
    testData->dltRingBuffer->idxWrite = testData->readIndex;
    safuRingBufferParam_t param = {
        .deleteEntries = true,
        .elements = BUFF_SIZE,
        .deleteFunc = elosTestDeleteEbLogEntry,
    };
    safuRingBufferNew(&testData->readQueue, &param);

    return 0;
}

int elosTestElosQueueNextLogEntriesSuccessEmptyTeardown(void **state) {
    struct elosTestData *testData = *state;
    safuRingBufferDelete(&testData->readQueue);
    testData->readQueue = NULL;
    return 0;
}

void elosTestElosQueueNextLogEntriesSuccessEmpty(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct elosTestData *testData = *state;
    SHOULD("try to read some dlt log entries from an empty dlt ring buffer");

    size_t copyReadIndex = testData->readIndex;

    result = elosQueueNextLogEntries(testData->dltRingBuffer, &testData->readIndex, testData->readQueue);

    assert_int_equal(testData->readIndex, copyReadIndex);
    assert_int_equal(result, SAFU_RESULT_NOT_FOUND);
    assert_int_equal(testData->readQueue->elementsWritten, 0);
}
