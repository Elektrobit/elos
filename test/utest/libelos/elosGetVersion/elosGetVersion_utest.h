// SPDX-License-Identifier: MIT
#ifndef __ELOS_ELOSGETVERSION_H__
#define __ELOS_ELOSGETVERSION_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "mock_libelos_communication.h"

#define MOCK_VERSIONSTR "1.2.3"

typedef struct elosUnitTestState {
    elosSession_t session;
    elosMessage_t *requestMsg;
    char const *normalResponseStr;
    char const *errorResponseStr;
    char const *emptyResponseStr;
} elosUnitTestState_t;

void elosMockSendMessage(elosUnitTestState_t *test, safuResultE_t result);
void elosMockReceiveJsonMessage(elosUnitTestState_t *test, char const *responseStr, safuResultE_t result);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosGetVersionErrCall)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosGetVersionErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosGetVersionSuccess)

#endif /* __ELOS_ELOSGETVERSION_H__ */
