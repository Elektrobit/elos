// SPDX-License-Identifier: MIT

#include "libelos_communication.h"

#include <stdlib.h>

#include "elos/libelos/libelos.h"
#include "libelos_constructor.h"
#include "safu/log.h"

safuResultE_t elosCreateMessage(uint8_t id, char const *jsonStr, elosMessage_t **message) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (message == NULL) {
        if (elosLoggingEnabled) safuLogErr("Invalid parameter");
    } else {
        elosMessage_t *newMessage;
        size_t messageLen = sizeof(elosMessage_t);
        ssize_t jsonLen = 0;

        if (message != NULL) {
            if (jsonStr != NULL) {
                jsonLen = strlen(jsonStr) + 1;
                messageLen += jsonLen;
            }

            newMessage = safuAllocMem(NULL, messageLen);
            if (newMessage == NULL) {
                if (elosLoggingEnabled) safuLogErr("Memory allocation failed");
            } else {
                memset(newMessage, 0, messageLen);
                newMessage->version = ELOS_PROTOCOL_VERSION;
                newMessage->message = id;
                newMessage->length = jsonLen;
                if (jsonStr != NULL) {
                    memcpy(newMessage->json, jsonStr, jsonLen);
                }

                *message = newMessage;
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}

safuResultE_t elosSendMessage(elosSession_t *session, elosMessage_t const *message) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool retVal;

    retVal = elosSessionValid(session);
    if (retVal == false) {
        if (elosLoggingEnabled) safuLogErr("Invalid session");
    } else if (message == NULL) {
        if (elosLoggingEnabled) safuLogErr("Invalid parameter");
    } else {
        size_t const messageLen = sizeof(elosMessage_t) + message->length;
        size_t bytes;

        result = safuSendExactly(session->fd, message, messageLen, &bytes);
        if (result != SAFU_RESULT_OK) {
            if (result != SAFU_RESULT_CLOSED) {
                if (elosLoggingEnabled)
                    safuLogErrF("Sending message failed with %d, %ld of %ld bytes sent.", result, bytes, messageLen);
            } else {
                if (elosLoggingEnabled) safuLogDebug("Connection has been closed");
            }

            result = SAFU_RESULT_FAILED;
            session->connected = false;
        }
    }

    return result;
}

safuResultE_t elosReceiveMessage(elosSession_t *session, elosMessage_t **message) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool retVal;

    retVal = elosSessionValid(session);
    if (retVal == false) {
        if (elosLoggingEnabled) safuLogErr("Invalid session");
    } else if (message == NULL) {
        if (elosLoggingEnabled) safuLogErr("Invalid parameter");
    } else {
        size_t messageLen = sizeof(elosMessage_t);
        elosMessage_t *newMessage;

        newMessage = safuAllocMem(NULL, messageLen);
        if (newMessage == NULL) {
            if (elosLoggingEnabled) safuLogDebug("Memory allocation failed");
        } else {
            size_t bytes;

            result = safuRecvExactly(session->fd, newMessage, messageLen, &bytes);
            if (result != SAFU_RESULT_OK) {
                if (result != SAFU_RESULT_CLOSED) {
                    if (elosLoggingEnabled)
                        safuLogErrF("Receiving message header failed with %d, %ld of %ld bytes received.", result,
                                    bytes, messageLen);
                } else {
                    if (elosLoggingEnabled) safuLogDebug("Connection has been closed");
                }
            } else if (newMessage->length > 0) {
                messageLen += newMessage->length;

                newMessage = safuAllocMem(newMessage, messageLen);
                if (newMessage == NULL) {
                    if (elosLoggingEnabled) safuLogErr("Memory allocation failed");
                    result = SAFU_RESULT_FAILED;
                } else {
                    result = safuRecvExactly(session->fd, newMessage->json, newMessage->length, &bytes);
                    if (result != SAFU_RESULT_OK) {
                        if (result != SAFU_RESULT_CLOSED) {
                            if (elosLoggingEnabled)
                                safuLogErrF("Receiving message body failed with %d, %ld of %d bytes received.", result,
                                            bytes, newMessage->length);
                        } else {
                            if (elosLoggingEnabled) safuLogDebug("Connection has been closed");
                        }
                    }
                }
            }

            if (result == SAFU_RESULT_OK) {
                *message = newMessage;
            } else {
                result = SAFU_RESULT_FAILED;
                session->connected = false;
                free(newMessage);
            }
        }
    }

    return result;
}

safuResultE_t elosSendJsonMessage(elosSession_t *session, uint8_t messageId, json_object *jsonObject) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool retVal;

    retVal = elosSessionValid(session);
    if (retVal == false) {
        if (elosLoggingEnabled) safuLogErr("Invalid session");
    } else if (jsonObject == NULL) {
        if (elosLoggingEnabled) safuLogErr("Invalid parameter");
    } else {
        const char *jsonString;

        jsonString = json_object_to_json_string_ext(jsonObject, JSON_C_TO_STRING_PLAIN);
        if (jsonString == NULL) {
            if (elosLoggingEnabled) safuLogErr("Converting json_object to string failed!");
        } else {
            elosMessage_t *message = NULL;

            result = elosCreateMessage(messageId, jsonString, &message);
            if (result != SAFU_RESULT_OK) {
                if (elosLoggingEnabled) safuLogErr("Creating message failed!");
            } else {
                result = elosSendMessage(session, message);
                // No check as there is no need to print "sending message failed" two times
            }

            free(message);
        }
    }

    return result;
}

safuResultE_t elosReceiveJsonMessage(elosSession_t *session, uint8_t messageId, json_object **jsonObject) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool retVal;

    retVal = elosSessionValid(session);
    if (retVal == false) {
        if (elosLoggingEnabled) safuLogErr("Invalid session");
    } else {
        elosMessage_t *message = NULL;
        json_object *newJsonObject = NULL;
        safuResultE_t resVal;

        resVal = elosReceiveMessage(session, &message);
        if (resVal != SAFU_RESULT_OK) {
            if (elosLoggingEnabled) safuLogErr("Receiving message failed!");
        } else if (message->message != messageId) {
            char const errStr[] = "Received message has invalid id! (expected 0x%x, received 0x%x)";
            if (elosLoggingEnabled) safuLogErrF(errStr, messageId, message->message);
        } else if (message->length == 0) {
            if (elosLoggingEnabled) safuLogErr("Received message has no payload!");
        } else {
            newJsonObject = json_tokener_parse(message->json);
            if (newJsonObject == NULL) {
                if (elosLoggingEnabled) safuLogErr("Parsing the response messages json string failed!");
            } else {
                json_object *jError = NULL;
                json_bool jsonRet;

                // Note: We can't use safuJsonGetString here as it returns -1 with error=NULL
                jsonRet = json_object_object_get_ex(newJsonObject, "error", &jError);
                if (jsonRet == false) {
                    // FIXME: Added due to inconsistent response generation, see #15517 for more details
                    result = SAFU_RESULT_OK;
                    // if(elosLoggingEnabled) safuLogErr("Invalid response message, error field is missing!");
                } else {
                    const char *errStr;

                    errStr = json_object_get_string(jError);
                    if (errStr != NULL) {
                        if (elosLoggingEnabled) safuLogErrF("elosd command failed with '%s'!", errStr);
                    } else {
                        result = SAFU_RESULT_OK;
                    }
                }
            }
        }

        if ((result == SAFU_RESULT_OK) && (jsonObject != NULL)) {
            *jsonObject = newJsonObject;
        } else {
            json_object_put(newJsonObject);
        }

        free(message);
    }

    return result;
}

safuResultE_t elosSendAndReceiveJsonMessage(elosSession_t *session, uint8_t messageId, json_object *sendJsonObject,
                                            json_object **receiveJsonObject) {
    safuResultE_t result;

    result = elosSendJsonMessage(session, messageId, sendJsonObject);
    if (result == SAFU_RESULT_OK) {
        uint8_t const receiveMessageId = (messageId | ELOS_MESSAGE_RESPONSE_BIT);

        result = elosReceiveJsonMessage(session, receiveMessageId, receiveJsonObject);
    }

    return result;
}
