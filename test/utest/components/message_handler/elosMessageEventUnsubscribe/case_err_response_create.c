// SPDX-License-Identifier: MIT

#include "elosMessageEventUnsubscribe_utest.h"
#include "mock_event_vector.h"
#include "mock_eventprocessor.h"
#include "safu/mock_safu.h"

int elosTestElosMessageEventUnsubscribeErrResponseCreateSetup(void **state) {
    elosMessageEventUnsubscribeUtestSetupEventProcessor(state);

    return 0;
}

int elosTestElosMessageEventUnsubscribeErrResponseCreateTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventUnsubscribeErrResponseCreate(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosMessageEventUnsubscribe");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when response creation fails");

    elosMessageEventUnsubscribeUtestSetupMessage(state, "{\"eventQueueId\" : 1}");

    PARAM("%s", "elosMessageHandlerResponseCreate fails");

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerResponseCreate, 0);
    expect_value(elosMessageHandlerResponseCreate, errstr, NULL);
    will_return(elosMessageHandlerResponseCreate, NULL);

    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
