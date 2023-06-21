// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorProcess_utest.h"

int elosTestEloEventFilterNodeVectorProcessSuccessSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const fStrA[] = {".event.messageCode 1000 EQ"};
    char const fStrB[] = {".event.hardwareid 'test123' STRCMP"};

    memset(test, 0, sizeof(elosTestState_t));
    elosEventFilterNodeVectorSetup(test, fStrA, fStrB);

    return 0;
}

int elosTestEloEventFilterNodeVectorProcessSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventFilterNodeVectorTeardown(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorProcessSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t const eventA = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test234"};
    elosEvent_t const eventB = {.messageCode = ELOS_MSG_CODE_WILDCARD_2XXX, .hardwareid = "test123"};
    elosEvent_t const eventC = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test123"};
    elosEvent_t const eventD = {.messageCode = ELOS_MSG_CODE_WILDCARD_3XXX, .hardwareid = "test456"};

    TEST("elosEventFilterNodeVectorProcess");
    SHOULD("%s", "add events to the eventQueue based on filters");

    // Note: As this functions calls elosEventFilterNodeProcess internally (which has its own in-depth unit test),
    //       we only test that that the events end up in their respective EventQueues here (and not their content)

    elosEventFilterNodeVectorTestStep(test, "eventA, matching rule 0", &eventA, NULL, 1, 0);
    elosEventFilterNodeVectorTestStep(test, "eventB, matching rule 1", &eventB, NULL, 1, 1);
    elosEventFilterNodeVectorTestStep(test, "eventC, matching rule 0 and 1", &eventC, NULL, 2, 2);
    elosEventFilterNodeVectorTestStep(test, "eventD, matching no rules", &eventD, NULL, 2, 2);
}
