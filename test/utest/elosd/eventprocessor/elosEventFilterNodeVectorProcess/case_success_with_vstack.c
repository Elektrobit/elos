// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorProcess_utest.h"

int elosTestEloEventFilterNodeVectorProcessSuccessWithVStackSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const fStrA[] = {".event.hardwareid .1 STRCMP"};
    char const fStrB[] = {".event.messageCode .2 EQ"};

    memset(test, 0, sizeof(elosTestState_t));
    elosEventFilterNodeVectorSetup(test, fStrA, fStrB);

    return 0;
}

int elosTestEloEventFilterNodeVectorProcessSuccessWithVStackTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventFilterNodeVectorTeardown(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorProcessSuccessWithVStack(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t const eventA = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test234"};
    elosEvent_t const eventB = {.messageCode = ELOS_MSG_CODE_WILDCARD_2XXX, .hardwareid = "test123"};
    elosEvent_t const eventC = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test123"};
    elosEvent_t const eventD = {.messageCode = ELOS_MSG_CODE_WILDCARD_3XXX, .hardwareid = "test456"};
    ELOS_FILTERSTACK_NEW(vStack, 2);
    ELOS_FILTERSTACK_SET(vStack, 0, RPNFILTER_VALUE_STRING, 7, "test123");
    ELOS_FILTERSTACK_SET_I32(vStack, 1, ELOS_MSG_CODE_WILDCARD_1XXX);

    TEST("elosEventFilterNodeVectorProcess (with valueStack)");
    SHOULD("%s", "add events to the eventQueue based on filters");

    // Note: As this functions calls elosEventFilterNodeProcess internally (which has its own in-depth unit test),
    //       we only test that that the events end up in their respective EventQueues here (and not their content)

    elosEventFilterNodeVectorTestStep(test, "eventA, matching rule 0", &eventA, vStack, 0, 1);
    elosEventFilterNodeVectorTestStep(test, "eventB, matching rule 1", &eventB, vStack, 1, 1);
    elosEventFilterNodeVectorTestStep(test, "eventC, matching rule 0 and 1", &eventC, vStack, 2, 2);
    elosEventFilterNodeVectorTestStep(test, "eventD, matching no rules", &eventD, vStack, 2, 2);
}
