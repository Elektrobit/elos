// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>

#include "dlt_hv/shmem_ring_buffer.h"
#include "elosQueueNextLogEntries_utest.h"

int elosTestElosQueueNextLogEntriesSuccessSetup(void **state) {
    struct elosTestData *testData = *state;
    safuRingBufferParam_t param = {
        .deleteEntries = true,
        .elements = BUFF_SIZE,
        .deleteFunc = elosTestDeleteEbLogEntry,
    };
    safuRingBufferNew(&testData->readQueue, &param);
    return 0;
}

int elosTestElosQueueNextLogEntriesSuccessTeardown(void **state) {
    struct elosTestData *testData = *state;
    safuRingBufferDelete(&testData->readQueue);
    testData->readQueue = NULL;
    return 0;
}

void elosTestElosQueueNextLogEntriesSuccess(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct elosTestData *testData = *state;
    SHOULD("successfully read some dlt log entries from the dlt ring buffer and push them into the queue!");

    result = elosQueueNextLogEntries(testData->dltRingBuffer, &testData->readIndex, testData->readQueue);

    assert_int_equal(testData->readIndex, BUFF_SIZE - 1);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testData->readQueue->elementsWritten, BUFF_SIZE - 1);
}
