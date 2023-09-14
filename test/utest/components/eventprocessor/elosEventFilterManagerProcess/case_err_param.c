// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerProcess_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterManagerProcessErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterManagerProcessErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterManagerProcessErrParam(UNUSED void **state) {
    elosEventFilterManager_t eventFilterManager = {.flags = SAFU_FLAG_INITIALIZED_BIT};
    elosEventFilterManager_t zeroedEventFilterManager = {0};
    elosEvent_t const event = {0};
    safuResultE_t result;

    TEST("elosEventFilterManagerProcess");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventFilterManagerProcess(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventFilterManager, NULL");
    result = elosEventFilterManagerProcess(&eventFilterManager, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &event");
    result = elosEventFilterManagerProcess(NULL, &event);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "zeroedEventFilterManager, &event");
    result = elosEventFilterManagerProcess(&zeroedEventFilterManager, &event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
