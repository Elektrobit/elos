// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeProcess_utest.h"

int elosTestEloEventFilterNodeProcessErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeProcessErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeProcessErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventQueue_t eventQueue = {0};
    elosEventFilterNode_t eventFilterNode = {.eventQueue = &eventQueue};
    elosEventFilterNode_t invalidEventFilterNode = {0};
    elosEvent_t const event = {0};

    TEST("elosEventFilterNodeProcess");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    // Note: We do not need to test the second parameter here, as NULL is also a valid value in this case

    PARAM("%s", "NULL, NULL, NULL");
    result = elosEventFilterNodeProcess(NULL, NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidEventFilterNode, NULL, NULL");
    result = elosEventFilterNodeProcess(&invalidEventFilterNode, NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&eventFilterNode, NULL, NULL");
    result = elosEventFilterNodeProcess(&eventFilterNode, NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, NULL, &event");
    result = elosEventFilterNodeProcess(NULL, NULL, &event);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidEventFilterNode, NULL, &event");
    result = elosEventFilterNodeProcess(&invalidEventFilterNode, NULL, &event);
    assert_true(result == SAFU_RESULT_FAILED);
}
