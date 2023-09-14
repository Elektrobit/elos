// SPDX-License-Identifier: MIT

#include "elosEventPublish_utest.h"

int elosTestElosEventPublishSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventPublishSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventPublishSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosUnitTestSet_t *tSet;
    safuResultE_t result;

    TEST("elosEventPublish");
    SHOULD("%s", "successfully publish an event");

    tSet = &test->normal;
    elosMockSendAndReceiveJsonMessage(test, tSet, SAFU_RESULT_OK);
    result = elosEventPublish(&test->session, &tSet->event);
    assert_int_equal(result, SAFU_RESULT_OK);
}
