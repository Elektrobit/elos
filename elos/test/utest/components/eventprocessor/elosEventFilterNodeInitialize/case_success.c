// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeInitialize_utest.h"

#define _FILTERID 42

int elosTestEloEventFilterNodeInitializeSuccessSetup(void **state) {
    *state = calloc(1, sizeof(elosTestState_t));

    return 0;
}

int elosTestEloEventFilterNodeInitializeSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeDeleteMembers(&test->efe);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeInitializeSuccess(void **state) {
    elosTestState_t *test = *state;
    elosEventQueue_t eventQueue = {0};
    char const *fStr[] = {".event.source.appName 'test' STRCMP", "1 1 AND"};
    size_t fCnt = ARRAY_SIZE(fStr);
    elosEventFilterNodeParam_t const param = {
        .filterStrings = fStr, .filterStringCount = fCnt, .eventQueue = &eventQueue, .id = _FILTERID};
    safuResultE_t result;
    elosEventFilter_t *eventFilter;

    TEST("elosEventFilterNodeInitialize");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeInitialize");

    result = elosEventFilterNodeInitialize(&test->efe, &param);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(safuVecElements(&test->efe.eventFilterVector) == fCnt);
    assert_true(test->efe.eventQueue == &eventQueue);
    assert_true(test->efe.id == _FILTERID);

    eventFilter = safuVecGet(&test->efe.eventFilterVector, 0);
    assert_non_null(eventFilter);
    assert_true(eventFilter->state == RPNFILTER_FINALIZED);

    eventFilter = safuVecGet(&test->efe.eventFilterVector, 0);
    assert_non_null(eventFilter);
    assert_true(eventFilter->state == RPNFILTER_FINALIZED);
}
