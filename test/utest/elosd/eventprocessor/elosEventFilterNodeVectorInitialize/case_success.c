// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorInitialize_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorInitializeSuccessSetup(void **state) {
    *state = calloc(1, sizeof(elosTestState_t));

    return 0;
}

int elosTestEloEventFilterNodeVectorInitializeSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeVectorDeleteMembers(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorInitializeSuccess(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeVectorInitialize");

    result = elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(test->efev.elementCount == 0);
    assert_true(test->efev.elementSize == sizeof(elosEventFilterNode_t));
    assert_true(test->efev.memorySize == sizeof(elosEventFilterNode_t) * _VECTORELEMENTS);
}
