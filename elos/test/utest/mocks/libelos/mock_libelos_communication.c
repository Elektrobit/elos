// SPDX-License-Identifier: MIT

#include "mock_libelos_communication.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosCreateMessage, safuResultE_t, uint8_t id, char const *jsonStr, elosMessage_t **message) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosCreateMessage)) {
        check_expected(id);
        check_expected_ptr(jsonStr);
        check_expected_ptr(message);
        if (message != NULL) {
            *message = mock_type(elosMessage_t *);
        }
        result = mock_type(int);
    } else {
        result = MOCK_FUNC_REAL(elosCreateMessage)(id, jsonStr, message);
    }

    return result;
}

MOCK_FUNC_BODY(elosSendMessage, safuResultE_t, elosSession_t *session, elosMessage_t const *message) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosSendMessage)) {
        check_expected_ptr(session);
        check_expected_ptr(message);
        result = mock_type(int);
    } else {
        result = MOCK_FUNC_REAL(elosSendMessage)(session, message);
    }

    return result;
}

MOCK_FUNC_BODY(elosReceiveMessage, safuResultE_t, elosSession_t *session, elosMessage_t **message) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosReceiveMessage)) {
        check_expected_ptr(session);
        check_expected_ptr(message);
        if (message != NULL) {
            *message = mock_type(elosMessage_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosReceiveMessage)(session, message);
    }

    return result;
}

MOCK_FUNC_BODY(elosSendJsonMessage, safuResultE_t, elosSession_t *session, uint8_t messageId, json_object *jsonObject) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosSendJsonMessage)) {
        check_expected_ptr(session);
        check_expected(messageId);
        check_expected_ptr(jsonObject);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosSendJsonMessage)(session, messageId, jsonObject);
    }

    return result;
}

MOCK_FUNC_BODY(elosReceiveJsonMessage, safuResultE_t, elosSession_t *session, uint8_t messageId,
               json_object **jsonObject) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosReceiveJsonMessage)) {
        check_expected_ptr(session);
        check_expected(messageId);
        check_expected_ptr(jsonObject);
        if (jsonObject != NULL) {
            *jsonObject = mock_type(json_object *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosReceiveJsonMessage)(session, messageId, jsonObject);
    }

    return result;
}

MOCK_FUNC_BODY(elosSendAndReceiveJsonMessage, safuResultE_t, elosSession_t *session, uint8_t messageId,
               json_object *sendJsonObject, json_object **receiveJsonObject) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosSendAndReceiveJsonMessage)) {
        check_expected_ptr(session);
        check_expected(messageId);
        check_expected_ptr(sendJsonObject);
        check_expected_ptr(receiveJsonObject);
        if (receiveJsonObject != NULL) {
            *receiveJsonObject = mock_type(json_object *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosSendAndReceiveJsonMessage)(session, messageId, sendJsonObject, receiveJsonObject);
    }

    return result;
}
