// SPDX-License-Identifier: MIT

#include "mock_message_handler.h"

#include <stdbool.h>

MOCK_FUNC_BODY(elosMessageHandlerSend, safuResultE_t, elosClientConnection_t const *const conn, uint8_t messageId,
               const char *jsonStr) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosMessageHandlerSend)) {
        check_expected_ptr(conn);
        check_expected(messageId);
        check_expected_ptr(jsonStr);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosMessageHandlerSend)(conn, messageId, jsonStr);
    }

    return result;
}

MOCK_FUNC_BODY(elosMessageHandlerSendJson, safuResultE_t, elosClientConnection_t const *const conn, uint8_t messageId,
               json_object *jobj) {
    if (MOCK_IS_ACTIVE(elosMessageHandlerSendJson)) {
        check_expected_ptr(conn);
        check_expected(messageId);
        check_expected_ptr(jobj);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosMessageHandlerSendJson)(conn, messageId, jobj);
}

MOCK_FUNC_BODY(elosMessageEventPublish, safuResultE_t, elosClientConnection_t const *const conn,
               elosMessage_t const *const msg) {
    if (MOCK_IS_ACTIVE(elosMessageEventPublish)) {
        check_expected_ptr(conn);
        check_expected_ptr(msg);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosMessageEventPublish)(conn, msg);
}

MOCK_FUNC_BODY(elosMessageGetVersion, safuResultE_t, elosClientConnection_t const *const conn,
               elosMessage_t const *const msg) {
    if (MOCK_IS_ACTIVE(elosMessageGetVersion)) {
        check_expected_ptr(conn);
        check_expected_ptr(msg);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosMessageGetVersion)(conn, msg);
}

MOCK_FUNC_BODY(elosMessageLogFindEvent, safuResultE_t, elosClientConnection_t const *const conn,
               elosMessage_t const *const msg) {
    if (MOCK_IS_ACTIVE(elosMessageLogFindEvent)) {
        check_expected_ptr(conn);
        check_expected_ptr(msg);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosMessageLogFindEvent)(conn, msg);
}

MOCK_FUNC_BODY(elosMessageHandlerResponseCreate, json_object *, const char *errstr) {
    if (MOCK_IS_ACTIVE(elosMessageHandlerResponseCreate)) {
        check_expected_ptr(errstr);
        return mock_type(json_object *);
    }
    return MOCK_FUNC_REAL(elosMessageHandlerResponseCreate)(errstr);
}
