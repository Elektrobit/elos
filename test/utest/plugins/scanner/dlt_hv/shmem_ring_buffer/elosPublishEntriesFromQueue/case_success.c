// SPDX-License-Identifier: MIT

#include <cmocka_extensions/mock_extensions.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>

#include "dlt_hv/shmem_ring_buffer.h"
#include "dlt_hv/types.h"
#include "elosPublishEntriesFromQueue_utets.h"

#define NUM_EVENTS_PUBLISH 5
int elosTestElosPublishEntriesFromQueueSuccessSetup(UNUSED void **state) {
    struct elosTestData *testData = *state;
    for (int i = 0; i < NUM_EVENTS_PUBLISH; i++) {
        elosEbLogEntry_t *entry = malloc(sizeof(elosEbLogEntry_t));
        safuRingBufferWrite(testData->readQueue, (void *)entry);
    }
    return 0;
}

int elosTestElosPublishEntriesFromQueueSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPublishEntriesFromQueueSuccess(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct elosTestData *testData = *state;
    SHOULD("successfully read the dlt log entries from queue and publish them!");

    struct elosPublisher *publisher = NULL;

    assert_int_equal(testData->readQueue->elementsWritten, NUM_EVENTS_PUBLISH);

    for (int i = 0; i < NUM_EVENTS_PUBLISH; i++) {
        expect_any(elosTestStoreEvent, event);
        will_return(elosTestStoreEvent, SAFU_RESULT_OK);
        expect_value(elosTestPublishEvent, publisher, publisher);
        expect_any(elosTestPublishEvent, event);
        will_return(elosTestPublishEvent, SAFU_RESULT_OK);
    }

    result = elosPublishEntriesFromQueue(testData->readQueue, testData->plugin, publisher);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testData->readQueue->elementsWritten, 0);
}
