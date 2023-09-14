// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeProcess_utest.h"

int elosTestEloEventFilterNodeProcessSuccessWithVStackSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const *filterString[] = {".event.hardwareid .1 STRCMP", ".event.messageCode .2 EQ"};
    uint32_t filterCount = ARRAY_SIZE(filterString);
    elosEventFilterNodeSetup(test, filterString, filterCount);
    return 0;
}

int elosTestEloEventFilterNodeProcessSuccessWithVStackTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventFilterNodeTeardown(test);
    return 0;
}

void elosTestEloEventFilterNodeProcessSuccessWithVStack(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t const eventA = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test234"};
    elosEvent_t const eventB = {.messageCode = ELOS_MSG_CODE_WILDCARD_2XXX, .hardwareid = "test123"};
    elosEvent_t const eventC = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test123"};
    elosEvent_t const eventD = {.messageCode = ELOS_MSG_CODE_WILDCARD_3XXX, .hardwareid = "test456"};
    ELOS_FILTERSTACK_NEW(vStack, 2);
    ELOS_FILTERSTACK_SET(vStack, 0, RPNFILTER_VALUE_STRING, 7, "test123");
    ELOS_FILTERSTACK_SET_I32(vStack, 1, ELOS_MSG_CODE_WILDCARD_1XXX);

    TEST("elosEventFilterNodeProcess (with valueStack)");
    SHOULD("%s", "add events to the eventQueue based on filters/valueStack");

    elosEventFilterNodeTestStep(test, "eventA, matching rule 0", &eventA, vStack, 1, true);
    elosEventFilterNodeTestStep(test, "eventB, matching rule 1", &eventB, vStack, 2, true);
    elosEventFilterNodeTestStep(test, "eventC, matching rule 0 and 1", &eventC, vStack, 4, true);
    elosEventFilterNodeTestStep(test, "eventD, matching no rule", &eventD, vStack, 4, false);
}
