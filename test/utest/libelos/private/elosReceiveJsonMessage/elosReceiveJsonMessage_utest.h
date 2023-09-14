// SPDX-License-Identifier: MIT
#ifndef ELOS_LIBELOS_ELOSRECEIVEJSONMESSAGE_UTEST_H
#define ELOS_LIBELOS_ELOSRECEIVEJSONMESSAGE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "mock_libelos_communication.h"

typedef struct elosTestSet {
    json_object *jsonObject;
    char const *jsonStr;
    elosMessage_t *message;
    size_t messageSize;
} elosTestSet_t;

typedef struct elosUnitTestState {
    elosMessage_t *clonedMessage;
    elosSession_t session;
    elosTestSet_t normal;
    elosTestSet_t empty;
    elosTestSet_t error;
    elosTestSet_t nojson;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveJsonMessageErrJson)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveJsonMessageErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveJsonMessageErrReceive)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveJsonMessageSuccess)

safuResultE_t elosMessageClone(elosMessage_t **message, elosMessage_t const *from);
void elosMockReceive(elosUnitTestState_t *test, elosMessage_t *message, safuResultE_t result);

#endif /* ELOS_LIBELOS_ELOSRECEIVEJSONMESSAGE_UTEST_H */
