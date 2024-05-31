// SPDX-License-Identifier: MIT
#include "elosMessageEventQueueRead_utest.h"

#include "elos/eventprocessor/eventprocessor.h"

TEST_SUITE_FUNC_PROTOTYPES(_messageEventQueueReadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosMessageEventQueueReadErrRequest),
        TEST_CASE(elosTestElosMessageEventQueueReadErrQueueRead),
        TEST_CASE(elosTestElosMessageEventQueueReadErrResponseCreate),
        TEST_CASE(elosTestElosMessageEventQueueReadErrResponseSend),
        TEST_CASE(elosTestElosMessageEventQueueReadSuccess),
    };

    return RUN_TEST_SUITE(tests, _messageEventQueueReadUtest);
}

static int _messageEventQueueReadUtestSetup(void **state) {
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

static int _messageEventQueueReadUtestTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->message);
    free(*state);

    return 0;
}

void elosMessageEventQueueReadUtestSetupEventProcessor(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    char const *filterStrings[] = {".event.messageCode 2000 AND"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEvent_t event = {0};
    safuResultE_t result;

    memset(&test->eventProcessor, 0, sizeof(elosEventProcessor_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    result = elosEventProcessorFilterNodeCreateWithQueue(&test->eventProcessor, filterStrings, filterStringCount,
                                                         &test->eventQueueId, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_OK);

    event.messageCode = ELOS_MSG_CODE_WILDCARD_2XXX;
    result = elosEventProcessorPublish(&test->eventProcessor, &event);
    assert_int_equal(result, SAFU_RESULT_OK);

    event.messageCode = ELOS_MSG_CODE_WILDCARD_3XXX;
    result = elosEventProcessorPublish(&test->eventProcessor, &event);
    assert_int_equal(result, SAFU_RESULT_OK);
}

void elosMessageEventQueueReadUtestSetupMessage(void **state, char const *messageString) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    memset(test->message, 0, MOCK_MESSAGE_MEMORY_SIZE);
    test->message->version = ELOS_PROTOCOL_VERSION;
    test->message->message = ELOS_MESSAGE_EVENTQUEUE_READ;
    test->message->length = strlen(messageString);
    strcpy(test->message->json, messageString);
}