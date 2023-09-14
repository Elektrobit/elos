// SPDX-License-Identifier: MIT

#include "elosEventPublish_utest.h"

int elosTestElosEventPublishErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventPublishErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventPublishErrParam(UNUSED void **state) {
    elosSession_t invalidSession = {0};
    elosSession_t session = {.connected = true};
    elosEvent_t event = {0};
    safuResultE_t result;

    TEST("elosEventPublish (parameter error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventPublish(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&session, NULL");
    result = elosEventPublish(&session, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, NULL");
    result = elosEventPublish(&invalidSession, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &event");
    result = elosEventPublish(NULL, &event);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, &event");
    result = elosEventPublish(&invalidSession, &event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
