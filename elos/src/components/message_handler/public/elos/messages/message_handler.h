// SPDX-License-Identifier: MIT
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <json-c/json.h>

#include "elos/clientmanager/types.h"
#include "elos/common/message.h"
#include "safu/common.h"

safuResultE_t elosMessageHandlerSend(elosClientManagerConnection_t const *const conn, uint8_t messageId,
                                     const char *jsonStr);
safuResultE_t elosMessageHandlerSendJson(elosClientManagerConnection_t const *const conn, uint8_t messageId,
                                         json_object *jobj);
struct json_object *elosMessageHandlerResponseCreate(const char *errstr);

safuResultE_t elosMessageHandlerHandleMessage(elosClientManagerConnection_t const *const conn);

#endif
