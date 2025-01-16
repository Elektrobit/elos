// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeProcess_utest.h"

int elosTestEloEventFilterNodeProcessSuccessSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const *filterString[] = {".event.hardwareid 'test123' STRCMP", ".event.messageCode 1000 EQ"};
    uint32_t filterCount = ARRAY_SIZE(filterString);
    elosEventFilterNodeSetup(test, filterString, filterCount);
    return 0;
}

int elosTestEloEventFilterNodeProcessSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventFilterNodeTeardown(test);
    return 0;
}

void elosTestEloEventFilterNodeProcessSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t const eventA = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test234"};
    elosEvent_t const eventB = {.messageCode = ELOS_MSG_CODE_WILDCARD_2XXX, .hardwareid = "test123"};
    elosEvent_t const eventC = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test123"};
    elosEvent_t const eventD = {.messageCode = ELOS_MSG_CODE_WILDCARD_3XXX, .hardwareid = "test456"};

    TEST("elosEventFilterNodeProcess");
    SHOULD("%s", "add events to the eventQueue based on filters");

    elosEventFilterNodeTestStep(test, "eventA, matching rule 0", &eventA, NULL, 1, true);
    elosEventFilterNodeTestStep(test, "eventB, matching rule 1", &eventB, NULL, 2, true);
    elosEventFilterNodeTestStep(test, "eventC, matching rule 0 and 1", &eventC, NULL, 4, true);
    elosEventFilterNodeTestStep(test, "eventD, matching no rule", &eventD, NULL, 4, false);
}
