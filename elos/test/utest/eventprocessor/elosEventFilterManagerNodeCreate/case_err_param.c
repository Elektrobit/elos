// SPDX-License-Identifier: MIT

#include <math.h>

#include "elosEventFilterManagerNodeCreate_utest.h"

#define _VECTORELEMENTS   8
#define _NODECREATE_PARAM 5

int elosTestEloEventFilterManagerNodeCreateErrParamSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    safuResultE_t result;
    elosEventFilterManagerParam_t const param = {
        .elements = _VECTORELEMENTS,
        .eventQueueManager = &test->eqm,
        .idManager = NULL,
    };

    result = elosEventQueueManagerInitialize(&test->eqm, NULL);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventQueueManagerEntryNew(&test->eqm, &test->eqId);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventFilterManagerInitialize(&test->efm, &param);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterManagerNodeCreateErrParamTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterManagerDeleteMembers(&test->efm);
    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventFilterManagerNodeCreateErrParam(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;
    elosEventFilterNodeId_t efnId = ELOS_ID_INVALID;
    char const *filterStrings[] = {"1 1 AND", "2 2 OR"};
    uint32_t const combinations = ((uint32_t)pow(2, _NODECREATE_PARAM)) - 1;

    TEST("elosEventFilterManagerNodeCreate");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    for (uint32_t i = 0; i < combinations; i += 1) {
        elosEventFilterManager_t *pEfm = (i & (1 << 0)) ? &test->efm : NULL;
        char const **pFiStr = (i & (1 << 1)) ? filterStrings : NULL;
        size_t pFiStrCnt = (i & (1 << 2)) ? ARRAY_SIZE(filterStrings) : 0;
        elosEventQueueId_t pEqId = (i & (1 << 3)) ? test->eqId : ELOS_ID_INVALID;
        elosEventFilterNodeId_t *pEfnId = (i & (1 << 4)) ? &efnId : NULL;

        SHOULD("test correct behaviour with combination i:%u", i);
        result = elosEventFilterManagerNodeCreate(pEfm, pFiStr, pFiStrCnt, pEqId, pEfnId);
        assert_true(result == SAFU_RESULT_FAILED);
    }
}
