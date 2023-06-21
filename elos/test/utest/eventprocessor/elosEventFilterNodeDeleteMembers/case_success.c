// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeDeleteMembers_utest.h"

#define _FILTERID 42

int elosTestEloEventFilterNodeDeleteMembersSuccessSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    char const *fStr[] = {"1 1 AND", "2 2 OR"};
    elosEventFilterNodeParam_t const param = {
        .filterStrings = fStr, .filterStringCount = ARRAY_SIZE(fStr), .eventQueue = &test->eq, .id = _FILTERID};

    elosEventFilterNodeInitialize(&test->efe, &param);

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeDeleteMembersSuccessTeardown(void **state) {
    free(*state);
    return 0;
}

void elosTestEloEventFilterNodeDeleteMembersSuccess(void **state) {
    elosTestState_t *test = *state;
    elosEventFilterVector_t zeroedEventFilterVector = {0};
    elosEventFilterNode_t zeroedEventFilterNode = {0};
    safuResultE_t result;

    TEST("elosEventFilterNodeDeleteMembers");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeDeleteMembers");

    PARAM("%s", "delete members of initialized struct");

    result = elosEventFilterNodeDeleteMembers(&test->efe);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->efe.state, EVENTFILTERNODE_STATE_INVALID);
    assert_int_equal(test->efe.id, ELOS_ID_INVALID);
    assert_ptr_equal(test->efe.eventQueue, NULL);
    assert_memory_equal(&test->efe.eventFilterVector, &zeroedEventFilterVector, sizeof(elosEventFilterVector_t));

    PARAM("%s", "delete members a second time on the same struct");

    result = elosEventFilterNodeDeleteMembers(&test->efe);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->efe.state, EVENTFILTERNODE_STATE_INVALID);
    assert_int_equal(test->efe.id, ELOS_ID_INVALID);
    assert_ptr_equal(test->efe.eventQueue, NULL);
    assert_memory_equal(&test->efe.eventFilterVector, &zeroedEventFilterVector, sizeof(elosEventFilterVector_t));

    PARAM("%s", "delete members on a zeroed struct");

    memcpy(&test->efe, &zeroedEventFilterNode, sizeof(elosEventFilterNode_t));
    result = elosEventFilterNodeDeleteMembers(&zeroedEventFilterNode);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_memory_equal(&test->efe, &zeroedEventFilterNode, sizeof(elosEventFilterNode_t));
}
