// SPDX-License-Identifier: MIT
#ifndef ELOS_LIBELOS_ELOSSENDANDRECEIVEJSONMESSAGE_UTEST_H
#define ELOS_LIBELOS_ELOSSENDANDRECEIVEJSONMESSAGE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "mock_libelos_communication.h"

typedef struct elosTestSet {
    json_object *requestObj;
    char const *requestStr;
    json_object *responseObj;
    char const *responseStr;
    uint8_t requestId;
    uint8_t responseId;
} elosTestSet_t;

typedef struct elosUnitTestState {
    elosSession_t session;
    elosTestSet_t normal;
    elosTestSet_t empty;
    elosTestSet_t error;
    json_object *bufferObj;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendAndReceiveJsonMessageErrCall)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendAndReceiveJsonMessageErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendAndReceiveJsonMessageSuccess)

void elosMockSendJsonMessage(elosUnitTestState_t *test, uint8_t id, safuResultE_t result);
void elosMockReceiveJsonMessage(elosUnitTestState_t *test, uint8_t id, json_object *jObj, safuResultE_t result);

#endif /* ELOS_LIBELOS_ELOSSENDANDRECEIVEJSONMESSAGE_UTEST_H */
