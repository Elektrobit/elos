// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventfilternode.h"
#include "elosEventFilterNodeVectorDeleteMembers_utest.h"

#define _VECTORELEMENTS 2

int elosTestEloEventFilterNodeVectorDeleteMembersSuccessSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));

    elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    const char *string1 = ".event.source.appName 'test' STRCMP";
    const char *string2 = "1 1 AND";
    char const **filterStrings1 = &string1;
    char const **filterStrings2 = &string2;
    elosEventFilterNodeParam_t const p1 = {.filterStrings = filterStrings1, .filterStringCount = 1};
    elosEventFilterNodeParam_t const p2 = {.filterStrings = filterStrings2, .filterStringCount = 1};
    elosEventFilterNodeVectorPushByParam(&test->efev, &p1);
    elosEventFilterNodeVectorPushByParam(&test->efev, &p2);

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeVectorDeleteMembersSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    // don't free the created objects. If they are not freed already, the function failed the test

    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorDeleteMembersSuccess(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorDeleteMembers");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeVectorDeleteMembers");

    result = elosEventFilterNodeVectorDeleteMembers(&test->efev);
    assert_true(result == SAFU_RESULT_OK);
}
