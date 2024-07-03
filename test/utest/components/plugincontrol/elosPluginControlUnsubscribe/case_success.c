// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/result.h>
#include <samconf/samconf.h>

#include "elos/eventprocessor/types.h"
#include "elos/plugincontrol/types.h"
#include "elosPluginControlUnsubscribe_utest.h"
#include "mock_eventprocessor.h"

int elosTestElosPluginControlUnsubscribeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginControlUnsubscribeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPluginControlUnsubscribeSuccess(UNUSED void **state) {
    TEST("elosPluginControlUnsubscribe");
    SHOULD("Cancel and cleanup given subscription");

    elosEventProcessor_t eventProcessor = {0};
    elosSubscriber_t subscriber = {.eventProcessor = &eventProcessor};
    elosSubscription_t subscription = {.eventFilterNodeId = 42, .eventQueueId = 8};

    MOCK_FUNC_AFTER_CALL(elosEventProcessorFilterNodeRemove, 0);
    expect_value(elosEventProcessorFilterNodeRemove, eventProcessor, subscriber.eventProcessor);
    expect_value(elosEventProcessorFilterNodeRemove, eventFilterNodeId, subscription.eventFilterNodeId);
    will_return(elosEventProcessorFilterNodeRemove, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventProcessorQueueRemove, 0);
    expect_value(elosEventProcessorQueueRemove, eventProcessor, subscriber.eventProcessor);
    expect_value(elosEventProcessorQueueRemove, eventQueueId, subscription.eventQueueId);
    will_return(elosEventProcessorQueueRemove, SAFU_RESULT_OK);

    safuResultE_t result = elosPluginControlUnsubscribe(&subscriber, &subscription);
    assert_int_equal(result, SAFU_RESULT_OK);
}
