// SPDX-License-Identifier: MIT
#include <cstddef>

#include "subscribe_utest.h"

int elosTestSubscribeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestSubscribeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestSubscribeSuccess(UNUSED void **state) {
    using namespace elos;
    const char *testFilterRule = NULL;
    size_t testSize = 0;
    elosEventQueueId_t *testQueue = NULL;
    safuResultE_t result;
    Elos testObject;

    TEST("subscribe");
    SHOULD("%s", "Create a subscription message, send it, evaluate the response and return list of message queue ids");

    result = testObject.subscribe(&testFilterRule, testSize, testQueue);
    assert_int_equal(result, SAFU_RESULT_OK);
}
