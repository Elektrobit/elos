// SPDX-License-Identifier: MIT
#include "publish_utest.h"

int elosTestPublishSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestPublishSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPublishSuccess(UNUSED void **state) {
    using namespace elos;
    elosEvent_t *testEvent = NULL;
    elosResultE result;
    Elos testObject;

    TEST("publish");
    SHOULD("%s", "successfully publish an event");

    result = testObject.publish(testEvent);
    assert_int_equal(result, ELOS_RESULT_OK);
}
