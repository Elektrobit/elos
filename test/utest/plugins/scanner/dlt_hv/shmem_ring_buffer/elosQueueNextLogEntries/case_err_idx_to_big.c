// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/result.h>

#include "dlt_hv/shmem_ring_buffer.h"
#include "elosQueueNextLogEntries_utest.h"

int elosTestElosQueueNextLogEntriesErrIdxToBigSetup(void **state) {
    struct elosTestData *testData = *state;
    safuRingBufferParam_t param = {
        .deleteEntries = true,
        .elements = BUFF_SIZE,
        .deleteFunc = elosTestDeleteEbLogEntry,
    };
    safuRingBufferNew(&testData->readQueue, &param);
    testData->dltRingBuffer->idxWrite = BUFF_SIZE + 20;
    return 0;
}

int elosTestElosQueueNextLogEntriesErrIdxToBigTeardown(void **state) {
    struct elosTestData *testData = *state;
    safuRingBufferDelete(&testData->readQueue);
    testData->readQueue = NULL;
    return 0;
}

void elosTestElosQueueNextLogEntriesErrIdxToBig(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct elosTestData *testData = *state;
    SHOULD("successfully read some dlt log entries from the dlt ring buffer and push them into the queue!");

    size_t copyReadIndex = testData->readIndex;

    result = elosQueueNextLogEntries(testData->dltRingBuffer, &testData->readIndex, testData->readQueue);

    assert_int_equal(testData->readIndex, copyReadIndex);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(testData->readQueue->elementsWritten, 0);
}
