// SPDX-License-Identifier: MIT

#ifndef __MOCK_ELOS_ELOSD_MESSAGE_HANDLER_H__
#define __MOCK_ELOS_ELOSD_MESSAGE_HANDLER_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_ELOS_ELOSD_MESSAGE_HANDLER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/messages/message_handler.h"
#include "json-c/json.h"

MOCK_FUNC_PROTOTYPE(elosMessageHandlerSend, safuResultE_t, elosClientConnection_t const *const conn, uint8_t messageId,
                    const char *jsonStr)

MOCK_FUNC_PROTOTYPE(elosMessageHandlerSendJson, safuResultE_t, elosClientConnection_t const *const conn,
                    uint8_t messageId, json_object *jobj)

MOCK_FUNC_PROTOTYPE(elosMessageEventPublish, safuResultE_t, elosClientConnection_t const *const conn,
                    elosMessage_t const *const msg)

MOCK_FUNC_PROTOTYPE(elosMessageGetVersion, safuResultE_t, elosClientConnection_t const *const conn,
                    elosMessage_t const *const msg)

MOCK_FUNC_PROTOTYPE(elosMessageLogFindEvent, safuResultE_t, elosClientConnection_t const *const conn,
                    elosMessage_t const *const msg)

MOCK_FUNC_PROTOTYPE(elosMessageHandlerResponseCreate, json_object *, const char *errstr)

#endif /* __MOCK_ELOS_ELOSD_MESSAGE_HANDLER_H__ */
