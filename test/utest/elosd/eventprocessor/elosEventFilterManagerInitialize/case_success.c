// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventfilternode.h"
#include "elosEventFilterManagerInitialize_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterManagerInitializeSuccessSetup(void **state) {
    elosUnitTestState_t *test = calloc(1, sizeof(elosUnitTestState_t));

    elosEventQueueManagerInitialize(&test->eqm, NULL);
    *state = test;

    return 0;
}

int elosTestEloEventFilterManagerInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *state;

    elosEventFilterManagerDeleteMembers(&test->efm);
    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventFilterManagerInitializeSuccess(void **state) {
    elosUnitTestState_t *test = *state;
    safuResultE_t result;
    elosEventFilterManagerParam_t const param = {
        .elements = _VECTORELEMENTS,
        .eventQueueManager = &test->eqm,
        .idManager = &test->idm,
    };

    TEST("elosEventFilterManagerInitialize");
    SHOULD("%s", "test correct behaviour of elosEventFilterManagerInitialize");

    result = elosEventFilterManagerInitialize(&test->efm, &param);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(test->efm.eventFilterNodeVector.elementCount == 0);
    assert_true(test->efm.eventFilterNodeVector.memorySize == (sizeof(elosEventFilterNode_t) * _VECTORELEMENTS));
    assert_true(test->efm.eventQueueManager = &test->eqm);
    assert_true(test->efm.idManager = &test->idm);
}
