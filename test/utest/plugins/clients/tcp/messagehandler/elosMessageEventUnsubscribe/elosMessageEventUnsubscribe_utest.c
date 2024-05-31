// SPDX-License-Identifier: MIT
#include "elosMessageEventUnsubscribe_utest.h"

#include "cmocka_mocks/mock_jsonc.h"
#include "elos/eventprocessor/eventprocessor.h"

TEST_SUITE_FUNC_PROTOTYPES(elosMessageEventUnsubscribeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosMessageEventUnsubscribeErrFilterDelete),
        TEST_CASE(elosTestElosMessageEventUnsubscribeErrQueueDelete),
        TEST_CASE(elosTestElosMessageEventUnsubscribeErrRequest),
        TEST_CASE(elosTestElosMessageEventUnsubscribeErrResponseCreate),
        TEST_CASE(elosTestElosMessageEventUnsubscribeErrResponseSend),
        TEST_CASE(elosTestElosMessageEventUnsubscribeSuccess),
    };

    return RUN_TEST_SUITE(tests, elosMessageEventUnsubscribeUtest);
}

static int elosMessageEventUnsubscribeUtestSetup(void **state) {
    elosUnitTestState_t *test;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);
    memset(test, 0, sizeof(elosUnitTestState_t));

    test->message = safuAllocMem(NULL, MOCK_MESSAGE_MEMORY_SIZE);
    assert_non_null(test->message);

    test->sharedData.eventProcessor = &test->eventProcessor;
    test->connection.sharedData = &test->sharedData;

    *state = test;

    return 0;
}

static int elosMessageEventUnsubscribeUtestTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->message);
    free(*state);

    return 0;
}

void elosMessageEventUnsubscribeUtestSetupEventProcessor(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    char const *filterStrings[] = {".event.messageCode 2000 EQ"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    safuResultE_t result;

    memset(&test->eventProcessor, 0, sizeof(elosEventProcessor_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    result = elosEventProcessorFilterNodeCreateWithQueue(&test->eventProcessor, filterStrings, filterStringCount,
                                                         &test->eventQueueId, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_OK);
}

void elosMessageEventUnsubscribeUtestSetupMessage(void **state, char const *messageString) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    memset(test->message, 0, MOCK_MESSAGE_MEMORY_SIZE);
    test->message->version = ELOS_PROTOCOL_VERSION;
    test->message->message = ELOS_MESSAGE_EVENT_UNSUBSCRIBE;
    test->message->length = strlen(messageString);
    strcpy(test->message->json, messageString);
}
