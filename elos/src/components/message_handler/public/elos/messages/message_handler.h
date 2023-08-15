// SPDX-License-Identifier: MIT
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <json-c/json.h>

#include "elos/clientmanager/types.h"
#include "elos/common/message.h"
#include "safu/common.h"

safuResultE_t elosMessageHandlerSend(elosClientConnection_t const *const conn, uint8_t messageId, const char *jsonStr);
safuResultE_t elosMessageHandlerSendJson(elosClientConnection_t const *const conn, uint8_t messageId,
                                         json_object *jobj);
struct json_object *elosMessageHandlerResponseCreate(const char *errstr);

safuResultE_t elosMessageHandlerHandleMessage(elosClientConnection_t const *const conn);

#endif
