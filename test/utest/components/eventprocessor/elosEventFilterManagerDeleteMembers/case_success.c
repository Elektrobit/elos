// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerDeleteMembers_utest.h"
#include "mock_eventfilternodevector.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterManagerDeleteMembersSuccessSetup(void **state) {
    elosTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosTestState_t));
    assert_non_null(test);

    memset(test, 0, sizeof(elosTestState_t));

    elosEventFilterManagerParam_t param = {
        .elements = 3,
        .eventQueueManager = &test->eventQueueManager,
        .idManager = &test->idManager,
    };
    result = elosEventFilterManagerInitialize(&test->eventFilterManager, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterManagerDeleteMembersSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    free(*state);
    *state = NULL;

    return 0;
}

void elosTestEloEventFilterManagerDeleteMembersSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventFilterManager_t zeroedEventFilterManager = {0};
    elosEventFilterNodeVector_t zeroedEventFilterNodeVector = {0};
    safuResultE_t result;

    TEST("elosEventFilterManagerDeleteMembers");
    SHOULD("%s", "test correct behaviour of elosEventFilterManagerDeleteMembers");

    PARAM("%s", "initialized elosEventFilterManager_t");
    result = elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->eventFilterManager.flags, 0);
    assert_int_equal(test->eventFilterManager.idCount, 0);
    assert_ptr_equal(test->eventFilterManager.eventQueueManager, NULL);
    assert_ptr_equal(test->eventFilterManager.idManager, NULL);
    assert_memory_equal(&test->eventFilterManager.eventFilterNodeVector, &zeroedEventFilterNodeVector,
                        sizeof(elosEventFilterNodeVector_t));

    PARAM("%s", "second call on already free'd elosEventFilterManager_t");
    result = elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "zeroed elosEventFilterManager_t");
    result = elosEventFilterManagerDeleteMembers(&zeroedEventFilterManager);
    assert_int_equal(result, SAFU_RESULT_OK);
}