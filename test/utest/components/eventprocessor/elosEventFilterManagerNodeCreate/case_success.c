// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventfilternode.h"
#include "elosEventFilterManagerNodeCreate_utest.h"
#include "safu/vector.h"

#define _VECTORELEMENTS 8
#define _ITERATIONS     3

int elosTestEloEventFilterManagerNodeCreateSuccessSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    safuResultE_t result;
    elosEventFilterManagerParam_t const param = {
        .elements = _VECTORELEMENTS,
        .eventQueueManager = &test->eqm,
        .idManager = NULL,
    };

    result = elosEventQueueManagerInitialize(&test->eqm, NULL);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventFilterManagerInitialize(&test->efm, &param);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterManagerNodeCreateSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterManagerDeleteMembers(&test->efm);
    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventFilterManagerNodeCreateSuccess(void **state) {
    elosTestState_t *test = *state;
    elosEventFilterNodeId_t efnId = ELOS_ID_INVALID;
    char const *fStr[] = {"1 1 AND", "2 2 OR"};
    safuResultE_t result;

    TEST("elosEventFilterManagerNodeCreate");
    SHOULD("%s", "test correct behaviour of elosEventFilterManagerNodeCreate");

    for (uint32_t i = 0; i < _ITERATIONS; i += 1) {
        elosEventQueueId_t eqId = ELOS_ID_INVALID;
        elosEventFilterNode_t *efNode = NULL;
        elosEventQueue_t *evQueue = NULL;

        SHOULD("test correct behaviour, iteration %u", i);

        result = elosEventQueueManagerEntryNew(&test->eqm, &eqId);
        assert_true(result == SAFU_RESULT_OK);

        result = elosEventFilterManagerNodeCreate(&test->efm, fStr, ARRAY_SIZE(fStr), eqId, &efnId);
        assert_true(result == SAFU_RESULT_OK);
        assert_true(test->efm.idCount == (i + 1));
        assert_true(test->efm.eventFilterNodeVector.elementCount == (i + 1));
        result = elosEventQueueManagerEntryGet(&test->eqm, eqId, &evQueue);
        assert_non_null(evQueue);
        efNode = safuVecGet(&test->efm.eventFilterNodeVector, i);
        assert_non_null(efNode);
        assert_true(efNode->id == test->efm.idCount);
        assert_true(efNode->eventFilterVector.elementCount == ARRAY_SIZE(fStr));
        assert_true(efNode->eventQueue == evQueue);
        assert_true(efNode->id == test->efm.idCount);
        assert_int_not_equal(efnId, ELOS_ID_INVALID);
    }
}
