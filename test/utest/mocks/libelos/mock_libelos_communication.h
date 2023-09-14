// SPDX-License-Identifier: MIT
#ifndef ELOS_LIBELOS_LIBELOS_MOCK_H
#define ELOS_LIBELOS_LIBELOS_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_LIBELOS_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "libelos_communication.h"

MOCK_FUNC_PROTOTYPE(elosCreateMessage, safuResultE_t, uint8_t id, char const *jsonStr, elosMessage_t **message)

MOCK_FUNC_PROTOTYPE(elosSendMessage, safuResultE_t, elosSession_t *session, elosMessage_t const *message)

MOCK_FUNC_PROTOTYPE(elosReceiveMessage, safuResultE_t, elosSession_t *session, elosMessage_t **message)

MOCK_FUNC_PROTOTYPE(elosSendJsonMessage, safuResultE_t, elosSession_t *session, uint8_t messageId,
                    json_object *jsonObject)
MOCK_FUNC_PROTOTYPE(elosReceiveJsonMessage, safuResultE_t, elosSession_t *session, uint8_t messageId,
                    json_object **jsonObject)
MOCK_FUNC_PROTOTYPE(elosSendAndReceiveJsonMessage, safuResultE_t, elosSession_t *session, uint8_t messageId,
                    json_object *sendJsonObject, json_object **receiveJsonObject)

#endif /* ELOS_LIBELOS_LIBELOS_MOCK_H */
