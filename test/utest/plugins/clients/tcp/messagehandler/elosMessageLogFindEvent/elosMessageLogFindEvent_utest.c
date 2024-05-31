// SPDX-License-Identifier: MIT
#include "elosMessageLogFindEvent_utest.h"

#include <elos/eventlogging/LogAggregator.h>

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosMessageLogFindEventErrConnection),
                                       TEST_CASE(elosTestElosMessageLogFindEventErrMessage),
                                       TEST_CASE(elosTestElosMessageLogFindEventExtErrEventVectorInitialize),
                                       TEST_CASE(elosTestElosMessageLogFindEventExtErrEventVectorToJsonArr),
                                       TEST_CASE(elosTestElosMessageLogFindEventExtErrLogAggregatorFindEvents),
                                       TEST_CASE(elosTestElosMessageLogFindEventExtErrResponseCreateFailed),
                                       TEST_CASE(elosTestElosMessageLogFindEventExtErrRuleFrmMsgGetString),
                                       TEST_CASE(elosTestElosMessageLogFindEventExtErrRuleFrmMsgStrdup),
                                       TEST_CASE(elosTestElosMessageLogFindEventExtErrRuleFrmMsgTokenerParse),
                                       TEST_CASE(elosTestElosMessageLogFindEventSuccess)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

void elosMessageLogFindEventUtestCreateConnection(void **state) {
    elosUteststateT_t *testState = *state;

    testState->connection = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    assert_non_null(testState->connection);

    testState->connection->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    assert_non_null(testState->connection->sharedData);

    testState->connection->sharedData->logAggregator = safuAllocMem(NULL, sizeof(elosLogAggregator_t));
    assert_non_null(testState->connection->sharedData->logAggregator);
}

void elosMessageLogFindEventUtestFreeConnection(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->connection->sharedData->logAggregator);
    free(testState->connection->sharedData);
    free(testState->connection);
}

void elosMessageLogFindEventUtestCreateMessage(void **state) {
    elosUteststateT_t *testState = *state;
    char filterRule[] = "{\"filter\": \".event.name 'sshd' STRCMP\"}";
    size_t ruleLen = strlen(filterRule) + 1;

    elosMessage_t *msg = safuAllocMem(NULL, sizeof(elosMessage_t) + ruleLen);
    assert_non_null(msg);

    msg->version = 0;
    msg->message = ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT;
    msg->length = ruleLen;
    strcpy(msg->json, filterRule);

    testState->msg = msg;
}

void elosMessageLogFindEventUtestFreeMessage(void **state) {
    elosUteststateT_t *testState = *state;

    free(testState->msg);
}
