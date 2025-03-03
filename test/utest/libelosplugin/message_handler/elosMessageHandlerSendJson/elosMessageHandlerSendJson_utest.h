// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEHANDLERSENDJSON_H__
#define __ELOS_MESSAGEHANDLERSENDJSON_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>
#include <safu/common.h>
#include <safu/mock_safu.h>
#include <stdlib.h>

#include "elos/common/message.h"
#include "elos/libelosplugin/message_handler.h"
#include "mock_message_handler.h"

typedef struct {
    char *jsonString;
    json_object *jsonObject;
} elosUnitTestState_t;

elosUnitTestState_t *elosMessageHandlerSendJsonCreateTestData(const char *objectStr);
int elosMessageHandlerSendJsonFreeTestData(elosUnitTestState_t *data);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerSendJsonSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerSendJsonExterrJsonObject)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerSendJsonExterrSend)

#endif /* __ELOS_MESSAGEHANDLERSENDJSON_H__ */
