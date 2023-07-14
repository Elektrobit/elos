// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorRemoveById_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorRemoveByIdSuccessSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    const char *string1 = ".event.source.appName 'test' STRCMP";
    const char *string2 = "1 1 AND";
    char const **filterStrings1 = &string1;
    char const **filterStrings2 = &string2;
    elosEventFilterNodeParam_t const p1 = {.filterStrings = filterStrings1, .filterStringCount = 1, .id = 1};
    elosEventFilterNodeParam_t const p2 = {.filterStrings = filterStrings2, .filterStringCount = 1, .id = 2};
    elosEventFilterNodeVectorPushByParam(&test->efev, &p1);
    elosEventFilterNodeVectorPushByParam(&test->efev, &p2);

    *state = test;
    return 0;
}

int elosTestEloEventFilterNodeVectorRemoveByIdSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeVectorDeleteMembers(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorRemoveByIdSuccess(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeVectorInitialize");

    result = elosEventFilterNodeVectorRemoveById(&test->efev, 1);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(test->efev.elementCount == 1);

    elosEventFilterNode_t *filterNode = (elosEventFilterNode_t *)test->efev.data;
    assert_int_equal(filterNode->id, 2);
}
