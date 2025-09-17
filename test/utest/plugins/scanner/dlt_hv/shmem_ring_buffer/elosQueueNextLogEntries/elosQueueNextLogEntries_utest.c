// SPDX-License-Identifier: MIT

#include "elosQueueNextLogEntries_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosQueueNextLogEntriesUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosQueueNextLogEntriesSuccess),
        TEST_CASE(elosTestElosQueueNextLogEntriesSuccessEmpty),
        TEST_CASE(elosTestElosQueueNextLogEntriesErrIdxToBig),
    };

    return RUN_TEST_SUITE(tests, elosQueueNextLogEntriesUtest);
}

safuResultE_t elosTestDeleteEbLogEntry(void *entry) {
    return elosFreeEbLogEntry((elosEbLogEntry_t *)entry);
}

int elosQueueNextLogEntriesUtestSetup(void **state) {
    struct elosTestData *testData = malloc(sizeof(struct elosTestData));

    testData->dltRingBuffer = malloc(sizeof(elosEbLogRingBuffer_t) + BUFF_SIZE * sizeof(elosEbLogEntry_t));
    testData->dltRingBuffer->entryCount = BUFF_SIZE;
    testData->dltRingBuffer->idxRead = 0;
    testData->dltRingBuffer->idxWrite = BUFF_SIZE - 1;

    *state = testData;
    return 0;
}
int elosQueueNextLogEntriesUtestTeardown(void **state) {
    struct elosTestData *testData = *state;

    free(testData->dltRingBuffer);
    testData->dltRingBuffer = NULL;
    free(testData);
    testData = NULL;
    return 0;
}
