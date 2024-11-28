// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerNodeRemove_utest.h"
#include "mock_eventfilternodevector.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterManagerNodeRemoveByEventQueueIdSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *filterStrings[] = {"1 1 EQ", "2 1 AND"};
    safuResultE_t result;
    uint32_t elements;

    elosEventFilterManagerParam_t eventFilterManagerparam = {
        .eventQueueManager = &test->eventQueueManager,
        .idManager = NULL,
        .elements = 4,
    };

    result = elosEventQueueManagerInitialize(&test->eventQueueManager, NULL);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventQueueManagerEntryNew(&test->eventQueueManager, &test->eventQueueIdA);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventQueueManagerEntryNew(&test->eventQueueManager, &test->eventQueueIdB);
    assert_int_equal(result, SAFU_RESULT_OK);

    elements = safuVecElements(&test->eventQueueManager.eventQueueVector);
    assert_int_equal(elements, 2);

    result = elosEventFilterManagerInitialize(&test->eventFilterManager, &eventFilterManagerparam);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventFilterManagerNodeCreate(&test->eventFilterManager, filterStrings, ARRAY_SIZE(filterStrings),
                                              test->eventQueueIdA, &test->eventFilterNodeIdA);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventFilterManagerNodeCreate(&test->eventFilterManager, filterStrings, ARRAY_SIZE(filterStrings),
                                              test->eventQueueIdB, &test->eventFilterNodeIdB);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventFilterManagerNodeCreate(&test->eventFilterManager, filterStrings, ARRAY_SIZE(filterStrings),
                                              test->eventQueueIdA, &test->eventFilterNodeIdC);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventFilterManagerNodeCreate(&test->eventFilterManager, filterStrings, ARRAY_SIZE(filterStrings),
                                              test->eventQueueIdA, &test->eventFilterNodeIdD);
    assert_int_equal(result, SAFU_RESULT_OK);

    elements = safuVecElements(&test->eventFilterManager.eventFilterNodeVector);
    assert_int_equal(elements, 4);

    return 0;
}

int elosTestEloEventFilterManagerNodeRemoveByEventQueueIdSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventQueueManagerDeleteMembers(&test->eventQueueManager);
    assert_int_equal(result, SAFU_RESULT_OK);

    memset(test, 0, sizeof(elosUnitTestState_t));

    return 0;
}

void elosTestEloEventFilterManagerNodeRemoveByEventQueueIdSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventFilterNode_t *eventFilterNode;
    safuResultE_t result;
    uint32_t elements;

    TEST("elosEventFilterManagerNodeRemove");
    SHOULD("%s", "test correct behaviour of elosEventFilterManagerNodeRemove");

    result = elosEventFilterManagerNodeRemoveByEventQueueId(&test->eventFilterManager, test->eventQueueIdA);
    assert_int_equal(result, SAFU_RESULT_OK);

    elements = safuVecElements(&test->eventFilterManager.eventFilterNodeVector);
    assert_int_equal(elements, 1);

    eventFilterNode = safuVecGetLast(&test->eventFilterManager.eventFilterNodeVector);
    assert_non_null(eventFilterNode);
    assert_int_equal(eventFilterNode->eventQueue->id, test->eventQueueIdB);
}
