// SPDX-License-Identifier: MIT

#include <string.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/eventlogging/LogAggregator.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/logger/logger.h"
#include "elos/messages/message_handler.h"
#include "safu/common.h"
#include "safu/json.h"
#include "safu/log.h"

static void _create_security_event(elosEvent_t *event, elosMessage_t const *const msg) {
    int retval = 0;

    retval = clock_gettime(CLOCK_REALTIME, &event->date);
    if (retval < 0) {
        safuLogWarn("clock_gettime failed");
    }
    free(event->source.appName);
    event->source.appName = strdup("elosd");
    free(event->source.fileName);
    event->source.fileName = strdup("/usr/bin/elosd");
    event->source.pid = getpid();
    event->severity = ELOS_SEVERITY_WARN;
    free(event->hardwareid);
    event->hardwareid = strdup(safuGetHardwareId());
    event->classification = ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS | ELOS_CLASSIFICATION_IPC;
    event->messageCode = ELOS_MSG_CODE_UNAUTHORIZED_PUBLISHING;
    free(event->payload);
    event->payload = strdup(msg->json);
}

static void _create_blacklist_error_event(elosEvent_t *event, elosMessage_t const *const msg) {
    int retval = 0;

    retval = clock_gettime(CLOCK_REALTIME, &event->date);
    if (retval < 0) {
        safuLogWarn("clock_gettime failed");
    }
    free(event->source.appName);
    event->source.appName = strdup("elosd");
    free(event->source.fileName);
    event->source.fileName = strdup("/usr/bin/elosd");
    event->source.pid = getpid();
    event->severity = ELOS_SEVERITY_ERROR;
    free(event->hardwareid);
    event->hardwareid = strdup(safuGetHardwareId());
    event->classification = ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS;
    event->messageCode = ELOS_MSG_CODE_EVENT_BLACKLIST_FAILED;
    free(event->payload);
    event->payload = strdup(msg->json);
}

safuResultE_t elosMessageEventPublish(elosClientConnection_t *conn, elosMessage_t const *const msg) {
    const uint8_t messageId = ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH;
    struct json_object *jresponse;
    const char *errstr = NULL;
    elosEvent_t event = {0};
    safuResultE_t retval = SAFU_RESULT_OK;

    if (conn == NULL || msg == NULL) {
        retval = SAFU_RESULT_FAILED;
        errstr = "elosMessageEventPublish called with null parameters";
        safuLogErr(errstr);
    }

    if (retval == SAFU_RESULT_OK) {
        retval = elosEventDeserialize(&event, msg->json);
        if (retval == SAFU_RESULT_FAILED) {
            errstr = "elosEventDeserialize failed";
            safuLogErr(errstr);
        } else {
            bool retBool;

            retBool = elosEventIsDateEmpty(&event);
            if (retBool == true) {
                retval = clock_gettime(CLOCK_REALTIME, &event.date);
                if (retval < 0) {
                    retval = SAFU_RESULT_FAILED;
                    errstr = "clock_gettime failed";
                    safuLogErr(errstr);
                }
            }
        }

        if (conn->isTrusted == false) {
            if (conn->blacklist.state == RPNFILTER_FINALIZED) {
                elosRpnFilterResultE_t result = elosEventFilterExecute(&conn->blacklist, NULL, &event);
                if (result == RPNFILTER_RESULT_MATCH) {
                    _create_security_event(&event, msg);
                    errstr = "unauthorized publishing attempt";
                    safuLogErr(errstr);
                    elosLog(ELOS_MSG_CODE_UNAUTHORIZED_PUBLISHING, ELOS_SEVERITY_WARN,
                            ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS | ELOS_CLASSIFICATION_IPC, errstr);
                }
            } else {
                _create_blacklist_error_event(&event, msg);
                errstr = "event authorization failed";
                safuLogErr(errstr);
            }
        }

        if (retval == SAFU_RESULT_OK) {
            safuResultE_t bufferResult = elosEventBufferWrite(&conn->eventBuffer, &event);
            if (bufferResult != SAFU_RESULT_OK) {
                errstr = "Writing into the EventBuffer failed";
                retval = SAFU_RESULT_FAILED;
                safuLogErr(errstr);
            }

            safuResultE_t loggerResult = elosLogAggregatorAdd(conn->sharedData->logAggregator, &event);
            if (loggerResult != SAFU_RESULT_OK) {
                safuLogWarn("elosLogAggregatorAdd failed");
            }
        }

        elosEventDeleteMembers(&event);

        jresponse = elosMessageHandlerResponseCreate(errstr);
        if (jresponse == NULL) {
            safuLogErr("elosMessageEventPublish failed");
            retval = SAFU_RESULT_FAILED;
        } else {
            if (SAFU_RESULT_OK != elosMessageHandlerSendJson(conn, messageId, jresponse)) {
                retval = SAFU_RESULT_FAILED;
            }
            json_object_put(jresponse);
        }
    }

    return retval;
}
