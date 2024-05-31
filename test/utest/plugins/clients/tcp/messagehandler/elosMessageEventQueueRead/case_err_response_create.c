// SPDX-License-Identifier: MIT

#include "elosMessageEventQueueRead_utest.h"
#include "mock_event_vector.h"
#include "mock_eventprocessor.h"
#include "safu/mock_safu.h"

int elosTestElosMessageEventQueueReadErrResponseCreateSetup(void **state) {
    elosMessageEventQueueReadUtestSetupEventProcessor(state);

    return 0;
}

int elosTestElosMessageEventQueueReadErrResponseCreateTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventQueueReadErrResponseCreate(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosMessageEventQueueRead");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when response creation fails");

    elosMessageEventQueueReadUtestSetupMessage(state, "{\"eventQueueId\" : 1}");

    PARAM("%s", "elosMessageHandlerResponseCreate fails");

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerResponseCreate, 0);
    expect_value(elosMessageHandlerResponseCreate, errstr, NULL);
    will_return(elosMessageHandlerResponseCreate, NULL);

    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "safuJsonAddObject fails");

    MOCK_FUNC_AFTER_CALL(safuJsonAddObject, 1);
    expect_not_value(__wrap_safuJsonAddObject, jobj, NULL);
    expect_not_value(__wrap_safuJsonAddObject, name, NULL);
    expect_any(__wrap_safuJsonAddObject, jdata);
    will_return(__wrap_safuJsonAddObject, -1);

    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosMessageHandlerSendJson fails");
}
