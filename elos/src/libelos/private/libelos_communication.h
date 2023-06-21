// SPDX-License-Identifier: MIT
#ifndef LIBELOS_COMMUNICATION_H
#define LIBELOS_COMMUNICATION_H

#include <json-c/json.h>

#include "elos/common/message.h"
#include "elos/libelos/libelos.h"

safuResultE_t elosCreateMessage(uint8_t id, char const *jsonStr, elosMessage_t **message);

safuResultE_t elosSendMessage(elosSession_t *session, elosMessage_t const *message);

safuResultE_t elosReceiveMessage(elosSession_t *session, elosMessage_t **message);

safuResultE_t elosSendJsonMessage(elosSession_t *session, uint8_t messageId, json_object *jsonObject);

safuResultE_t elosReceiveJsonMessage(elosSession_t *session, uint8_t messageId, json_object **jsonObject);

safuResultE_t elosSendAndReceiveJsonMessage(elosSession_t *session, uint8_t messageId, json_object *sendJsonObject,
                                            json_object **receiveJsonObject);

#endif /* LIBELOS_COMMUNICATION_H */
