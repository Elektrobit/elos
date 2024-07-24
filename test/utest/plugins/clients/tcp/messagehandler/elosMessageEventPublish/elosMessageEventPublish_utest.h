// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEEVENTPUBLISH_H__
#define __ELOS_MESSAGEEVENTPUBLISH_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>

#include "connectionmanager/clientconnection_types.h"
#include "elos/common/message.h"

typedef struct elosUtestState {
    elosClientConnection_t *conn;
    elosMessage_t *msg;
    json_object *response;
    elosEvent_t *event;
} elosUtestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventPublishSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestelosMessageEventPublishBlacklistFilterSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestelosMessageEventPublishBlacklistFilterCreateError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventPublishConnectionNullError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventPublishMessageNullError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventPublishEventEmpty)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventPublishLoggingFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventPublishPublishFailed)

int elosMessageEventPublishCheckJsonObject(const long unsigned int parameter, const long unsigned int compare);
int elosMessageEventPublishCheckEvent(const elosEvent_t *expected, const elosEvent_t *actual);

#endif /* __ELOS_MESSAGEEVENTPUBLISH_H__ */
