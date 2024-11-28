// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/defines.h"
#include "elosEventProcessorInitialize_utest.h"
#include "mock_eventfilternodevector.h"

int elosTestEloEventProcessorInitializeSuccessSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    memset(test, 0, sizeof(elosTestState_t));
    return 0;
}

int elosTestEloEventProcessorInitializeSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorInitializeSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    elosEventProcessor_t *ep = &test->eventProcessor;
    safuResultE_t result;

    TEST("elosEventProcessorInitialize");
    SHOULD("%s", "Test successful code execution");

    result = elosEventProcessorInitialize(ep, &param);
    assert_true(result == SAFU_RESULT_OK);
    assert_ptr_equal(ep->eventFilterManager.eventQueueManager, &ep->eventQueueManager);
    assert_ptr_not_equal(ep->eventFilterManager.eventFilterNodeVector.data, NULL);
    assert_int_equal(ep->eventQueueManager.defaultEventQueueSize, ELOS_EVENTQUEUE_DEFAULT_SIZE);
    assert_ptr_not_equal(ep->eventQueueManager.eventQueueVector.data, NULL);
}
