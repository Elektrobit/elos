// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerNodeRemove_utest.h"
#include "mock_eventfilternodevector.h"

int elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventFilterManagerParam_t param = {
        .eventQueueManager = &test->eventQueueManager,
        .idManager = NULL,
        .elements = 8,
    };

    result = elosEventFilterManagerInitialize(&test->eventFilterManager, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    assert_int_equal(result, SAFU_RESULT_OK);

    memset(test, 0, sizeof(elosUnitTestState_t));

    return 0;
}

void elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventFilterManagerNodeRemove");
    SHOULD("%s", "fail as invalid parameter is given.");

    PARAM("%s", "NULL, 0");
    result = elosEventFilterManagerNodeRemoveByEventQueueId(NULL, 0);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventFilterManager, 0");
    result = elosEventFilterManagerNodeRemoveByEventQueueId(&test->eventFilterManager, 0);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, 42");
    result = elosEventFilterManagerNodeRemoveByEventQueueId(NULL, 42);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
