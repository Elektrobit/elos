// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <samconf/samconf.h>

#include "elos/plugincontrol/types.h"
#include "elosPluginControlUnsubscribe_utest.h"

int elosTestElosPluginControlUnsubscribeErrInvalidParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginControlUnsubscribeErrInvalidParameterTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPluginControlUnsubscribeErrInvalidParameter(UNUSED void **state) {
    elosEventProcessor_t eventProcessor = {0};
    elosSubscriber_t subscriber = {.eventProcessor = &eventProcessor};
    elosSubscription_t subscription = {.eventFilterNodeId = 42, .eventQueueId = 8};

    TEST("elosPluginControlUnsubscribe");
    SHOULD("fail if one of the provided parameters is invalid");

    safuResultE_t result = elosPluginControlUnsubscribe(&subscriber, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    result = elosPluginControlUnsubscribe(NULL, &subscription);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
