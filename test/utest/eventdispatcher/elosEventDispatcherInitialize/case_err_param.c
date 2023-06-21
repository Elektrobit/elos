// SPDX-License-Identifier: MIT

#include "elosEventDispatcherInitialize_utest.h"

#define _TEST_EVENTLIMITCOUNT 8

int testElosEventDispatcherInitializeErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {
        .eventProcessor = &test->eventProcessor,
    };

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherInitializeErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherDeleteMembers(&test->eventDispatcher);
    return 0;
}

void testElosEventDispatcherInitializeErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcher_t eventDispatcher = {0};
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    elosEventDispatcherParam_t const invalidParam = {.eventProcessor = NULL};

    TEST("elosEventDispatcherInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventDispatcherInitialize(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, param");
    result = elosEventDispatcherInitialize(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventDispatcher, NULL");
    result = elosEventDispatcherInitialize(&eventDispatcher, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventDispatcher, invalidParam");
    result = elosEventDispatcherInitialize(&eventDispatcher, &invalidParam);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "initializedEventDispatcher, NULL");
    result = elosEventDispatcherInitialize(&test->eventDispatcher, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
