// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

#include "elos/common/types.h"
#include "elos/eventprocessor/types.h"
#include "elos/plugincontrol/plugincontrol.h"
#include "elos/plugincontrol/types.h"
#include "elosPluginControlUnsubscribe_utest.h"
#include "mock_eventfiltermanager.h"
#include "mock_eventprocessor.h"
#include "mock_eventqueuemanager.h"

struct test_data {
    elosSubscriber_t *subscriber;
    elosSubscription_t *subscribtion;
};

int elosTestElosPluginControlUnsubscribeSuccessSetup(void **state) {
    struct test_data *data = safuAllocMem(NULL, sizeof(struct test_data));

    elosEventProcessor_t *eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    data->subscriber = safuAllocMem(NULL, sizeof(elosSubscriber_t));
    data->subscriber->eventProcessor = eventProcessor;
    safuVecCreate(&data->subscriber->subscriptions, 1, sizeof(elosSubscription_t *));

    char *filterStr = "1 1 EQ";

    MOCK_FUNC_AFTER_CALL(elosEventQueueManagerEntryNew, 0);
    expect_any(elosEventQueueManagerEntryNew, eqm);
    expect_any(elosEventQueueManagerEntryNew, eqId);
    will_return(elosEventQueueManagerEntryNew, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventFilterManagerNodeCreate, 0);
    expect_any(elosEventFilterManagerNodeCreate, efm);
    expect_any(elosEventFilterManagerNodeCreate, filterStrings);
    expect_value(elosEventFilterManagerNodeCreate, filterStringCount, 1);
    expect_any(elosEventFilterManagerNodeCreate, eqId);
    expect_any(elosEventFilterManagerNodeCreate, efnId);
    will_set_parameter(elosEventFilterManagerNodeCreate, efnId, 1);
    will_return(elosEventFilterManagerNodeCreate, SAFU_RESULT_OK);

    elosPluginControlSubscribe(data->subscriber, (const char *const *)&filterStr, 1,
                               (const elosSubscription_t **)&data->subscribtion);

    *state = data;
    return 0;
}

int elosTestElosPluginControlUnsubscribeSuccessTeardown(void **state) {
    struct test_data *data = *state;

    safuVecFree(&data->subscriber->subscriptions);
    free(data->subscriber->eventProcessor);
    free(data->subscriber);
    free(data);

    return 0;
}

void elosTestElosPluginControlUnsubscribeSuccess(void **state) {
    TEST("elosPluginControlUnsubscribe");
    SHOULD("Cancel and cleanup given subscription");

    struct test_data *data = *state;

    elosSubscriber_t *subscriber = data->subscriber;
    const elosSubscription_t *subscription = data->subscribtion;

    assert_int_equal(safuVecElements(&subscriber->subscriptions), 1);

    MOCK_FUNC_AFTER_CALL(elosEventProcessorFilterNodeRemove, 0);
    expect_value(elosEventProcessorFilterNodeRemove, eventProcessor, subscriber->eventProcessor);
    expect_value(elosEventProcessorFilterNodeRemove, eventFilterNodeId, subscription->eventFilterNodeId);
    will_return(elosEventProcessorFilterNodeRemove, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventProcessorQueueRemove, 0);
    expect_value(elosEventProcessorQueueRemove, eventProcessor, subscriber->eventProcessor);
    expect_value(elosEventProcessorQueueRemove, eventQueueId, subscription->eventQueueId);
    will_return(elosEventProcessorQueueRemove, SAFU_RESULT_OK);

    safuResultE_t result = elosPluginControlUnsubscribe(subscriber, subscription);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(safuVecElements(&subscriber->subscriptions), 0);
}
