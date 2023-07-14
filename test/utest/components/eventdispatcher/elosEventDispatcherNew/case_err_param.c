// SPDX-License-Identifier: MIT

#include "elosEventDispatcherNew_utest.h"

int testElosEventDispatcherNewErrParamSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherNewErrParamTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherNewErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    elosEventDispatcherParam_t const invalidParam = {.eventProcessor = NULL};

    TEST("elosEventDispatcherNew");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventDispatcherNew(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, param");
    result = elosEventDispatcherNew(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, invalidParam");
    result = elosEventDispatcherNew(NULL, &invalidParam);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventDispatcher, NULL");
    result = elosEventDispatcherNew(&test->eventDispatcher, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventDispatcher, invalidParam");
    result = elosEventDispatcherNew(&test->eventDispatcher, &invalidParam);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
