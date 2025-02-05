// SPDX-License-Identifier: MIT

#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <safu/vector_types.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "elos/event/event_types.h"
#include "elos/event/event_vector.h"
#define _DEFAULT_SOURCE 1

#include <netdb.h>
#include <safu/json.h>
#include <safu/log.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "elos/common/types.h"
#include "elos/event/event.h"
#include "elos/libelos/libelos.h"
#include "libelos_communication.h"

#define VERSION_DATA_LEN 128

bool elosSessionValid(elosSession_t const *session) {
    bool result = false;

    if ((session != NULL) && (session->connected == true)) {
        result = true;
    }

    return result;
}

safuResultE_t elosConnectSessionTcpip(char const *host, uint16_t port, elosSession_t *session) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal;
    char service[6];
    struct addrinfo *serverInfo;
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC, /* Allow IPv4 or IPv6 */
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP, /* Allow TCP/IP connections */
    };

    snprintf(service, ARRAY_SIZE(service), "%d", port);

    retVal = getaddrinfo(host, service, &hints, &serverInfo);
    if (retVal != 0) {
        safuLogErrF("Failed to fetch address info: %s\n", gai_strerror(retVal));
    } else {
        int sfd;
        struct addrinfo *ap;

        // iterate threw addresses returned by getaddrinfo()
        for (ap = serverInfo; ap != NULL; ap = ap->ai_next) {
            sfd = socket(ap->ai_family, ap->ai_socktype, ap->ai_protocol);
            if (sfd == -1) {
                continue;
            }

            retVal = connect(sfd, ap->ai_addr, ap->ai_addrlen);
            if (retVal != -1) {
                break; /* Success */
            }

            close(sfd);
        }

        freeaddrinfo(serverInfo);

        if (ap == NULL) {
            safuLogErrF("connect to %s:%d failed!", host, port);
        } else {
            result = SAFU_RESULT_OK;
            session->fd = sfd;
            session->connected = true;
        }
    }
    return result;
}

safuResultE_t elosConnectTcpip(char const *host, uint16_t port, elosSession_t **session) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((host == NULL) || (session == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosSession_t *newSession = NULL;

        newSession = safuAllocMem(NULL, sizeof(elosSession_t));
        if (newSession == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            result = elosConnectSessionTcpip(host, port, newSession);

            if (result == SAFU_RESULT_OK) {
                *session = newSession;
            } else {
                free(newSession);
            }
        }
    }
    return result;
}

safuResultE_t elosConnectUnix(char const *path, elosSession_t **session) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct sockaddr_un address;

    if ((path == NULL) || (session == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosSession_t *newSession = NULL;

        newSession = safuAllocMem(NULL, sizeof(elosSession_t));
        if (newSession == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            int ret;
            int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
            if (sfd == -1) {
                safuLogErr("socket set up failed");
            } else {
                memset(&address, 0, sizeof(struct sockaddr_un));
                address.sun_family = AF_UNIX;
                strncpy(address.sun_path, path, sizeof(address.sun_path) - 1);
                ret = connect(sfd, (struct sockaddr *)&address, sizeof(struct sockaddr_un));
                if (ret == -1) {
                    safuLogErrF("connect to path : %s failed!", path);
                } else {
                    result = SAFU_RESULT_OK;
                    newSession->fd = sfd;
                    newSession->connected = true;
                    *session = newSession;
                }

                if (result != SAFU_RESULT_OK) {
                    free(newSession);
                }
            }
        }
    }
    return result;
}

safuResultE_t elosDisconnectSession(elosSession_t *session) {
    safuResultE_t result = SAFU_RESULT_OK;

    int retVal = close(session->fd);
    if (retVal < 0) {
        safuLogErrErrno("close failed!");
        return SAFU_RESULT_FAILED;
    }

    session->fd = 0;
    session->connected = false;

    return result;
}

safuResultE_t elosDisconnect(elosSession_t *session) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (session != NULL) {
        result = elosDisconnectSession(session);
        free(session);
    }

    return result;
}

safuResultE_t elosGetVersion(elosSession_t *session, char const **version) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    static char versionStr[VERSION_DATA_LEN];
    bool retBool;

    retBool = elosSessionValid(session);
    if (retBool == false) {
        safuLogErr("Invalid session");
    } else if (version == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        safuResultE_t retResult;
        json_object *jResult = NULL;
        elosMessage_t const request = {
            .version = ELOS_PROTOCOL_VERSION,
            .message = ELOS_MESSAGE_GET_VERSION,
            .length = 0,
        };

        retResult = elosSendMessage(session, &request);
        if (retResult != SAFU_RESULT_OK) {
            safuLogErrErrno("Sending message failed");
        } else {
            retResult = elosReceiveJsonMessage(session, ELOS_MESSAGE_RESPONSE_GET_VERSION, &jResult);
            if (retResult != SAFU_RESULT_OK) {
                safuLogErrErrno("Receiving message failed");
            } else {
                char const *newVersionStr = NULL;
                int retInt;

                retInt = safuJsonGetString(jResult, "version", 0, &newVersionStr);
                if (retInt < 0) {
                    safuLogErrErrno("Reading version from message failed");
                } else {
                    strncpy(versionStr, newVersionStr, VERSION_DATA_LEN - 1);
                    *version = versionStr;
                    result = SAFU_RESULT_OK;
                }

                json_object_put(jResult);
            }
        }
    }

    return result;
}

// Deprecated
safuResultE_t elosLogFindEvent(elosSession_t *session, const char *filterRule, elosEventVector_t **eventVector) {
    return elosFindEvents(session, filterRule, &(struct timespec){0}, &(struct timespec){0}, eventVector);
}

static safuResultE_t _createFindEventRequest(const char *filterRule, struct timespec const *newest,
                                             struct timespec const *oldest, json_object **jRequest) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    json_object *newJsonRequest = json_object_new_object();
    if (newJsonRequest == NULL) {
        safuLogErr("json_object_new_object failed!");
    } else {
        json_object *jsonAttribute = NULL;
        jsonAttribute = safuJsonAddNewString(newJsonRequest, "filter", filterRule);
        if (jsonAttribute == NULL) {
            safuLogErr("safuJsonAddNewString failed!");
        } else {
            jsonAttribute = safuJsonAddNewTimestamp(newJsonRequest, "newest", newest);
            if (jsonAttribute == NULL) {
                safuLogErr("safuJsonAddNewTimestamp failed!");
            } else {
                jsonAttribute = safuJsonAddNewTimestamp(newJsonRequest, "oldest", oldest);
                if (jsonAttribute == NULL) {
                    safuLogErr("safuJsonAddNewTimestamp failed!");
                }
            }
        }

        if (jsonAttribute == NULL) {
            json_object_put(newJsonRequest);
        } else {
            *jRequest = newJsonRequest;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static safuResultE_t _readFindEventResponse(json_object *jResponse, elosEventVector_t **eventVector,
                                            bool *isTruncated) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    json_object *jIsTruncated = safuJsonGetObject(jResponse, "isTruncated", 0);
    if (jIsTruncated != NULL && json_object_get_type(jIsTruncated) == json_type_boolean) {
        *isTruncated = json_object_get_boolean(jIsTruncated);
    } else {
        *isTruncated = false;
    }

    json_object *eventVecJarr = safuJsonGetArray(jResponse, "eventArray", 0, NULL);
    if (!eventVecJarr) {
        safuLogErr("Failed to read event vector json object!");
    } else {
        elosEventVector_t *newEventVector = NULL;

        result = elosEventVectorFromJsonArray(eventVecJarr, &newEventVector);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("Failed to read event vector json object!");
        } else {
            if (*eventVector == NULL) {
                *eventVector = newEventVector;
            } else {
                size_t eventCount = safuVecElements(newEventVector);
                for (size_t i = 0; i < eventCount; i++) {
                    elosEventVectorPush(*eventVector, safuVecGet(newEventVector, i));
                }
                safuVecFree(newEventVector);
                free(newEventVector);
            }
            result = SAFU_RESULT_OK;
        }
    }
    return result;
}

safuResultE_t elosFindEvents(elosSession_t *session, const char *filterRule, struct timespec const *newest,
                             struct timespec const *oldest, elosEventVector_t **eventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool retBool;
    bool isTruncated = false;

    retBool = elosSessionValid(session);
    if (retBool == false) {
        safuLogErr("Invalid session");
    } else if ((filterRule == NULL) || (eventVector == NULL) || (newest == NULL) || (oldest == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        struct timespec currentOldest = *oldest;
        do {
            json_object *jRequest = NULL;
            result = _createFindEventRequest(filterRule, newest, &currentOldest, &jRequest);
            if (result == SAFU_RESULT_OK) {
                safuLogDebugF("will send filter rule: %s", filterRule);

                json_object *jResponse = NULL;
                result = elosSendAndReceiveJsonMessage(session, ELOS_MESSAGE_LOG_FIND_EVENT, jRequest, &jResponse);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("Communication with elosd failed!");
                } else {
                    result = _readFindEventResponse(jResponse, eventVector, &isTruncated);
                    if (result == SAFU_RESULT_OK && isTruncated) {
                        elosEvent_t *lastEvent = safuVecGetLast(*eventVector);
                        currentOldest = lastEvent->date;
                    }
                    json_object_put(jResponse);
                }

                json_object_put(jRequest);
            }
        } while (isTruncated);
    }

    return result;
}

safuResultE_t elosEventPublish(elosSession_t *session, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    bool retBool = elosSessionValid(session);
    if (retBool == false) {
        safuLogErr("Invalid session");
    } else if (event == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        json_object *eventObject = json_object_new_object();
        if (!eventObject) {
            safuLogErr("json_object_new_object failed!");
        } else {
            result = elosEventToJsonObject(eventObject, event);
        }

        if (result == SAFU_RESULT_OK) {
            uint8_t const requestId = ELOS_MESSAGE_EVENT_PUBLISH;
            result = elosSendAndReceiveJsonMessage(session, requestId, eventObject, NULL);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("Communication with elosd failed!");
            }
        }

        if (eventObject) {
            json_object_put(eventObject);
        }
    }

    return result;
}

safuResultE_t elosEventUnsubscribe(elosSession_t *session, elosEventQueueId_t eventQueueId) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool success;

    success = elosSessionValid(session);
    if (success == true) {
        json_object *request = json_object_new_object();
        if (request != NULL) {
            json_object *jsonQueueId = safuJsonAddNewUint64(request, "eventQueueId", eventQueueId);
            if (jsonQueueId != NULL) {
                uint8_t const requestId = ELOS_MESSAGE_EVENT_UNSUBSCRIBE;
                json_object *response = NULL;

                result = elosSendAndReceiveJsonMessage(session, requestId, request, &response);
                if (result == SAFU_RESULT_OK) {
                    json_object_put(response);
                } else {
                    safuLogErr("Communication with elosd failed.");
                }
            } else {
                safuLogErr("Failed to add eventQueueId to json object.");
            }

            json_object_put(request);
        } else {
            safuLogErr("Failed to create request json object.");
        }
    } else {
        safuLogErr("Invalid session.");
    }

    return result;
}

safuResultE_t elosEventSubscribe(elosSession_t *session, const char **filterRuleArray, size_t filterRuleArraySize,
                                 elosEventQueueId_t *eventQueueId) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool success;

    success = elosSessionValid(session);
    if (success == false) {
        safuLogErr("Invalid session");
    } else if ((filterRuleArray == NULL) || (filterRuleArraySize == 0) || (eventQueueId == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        json_object *request;

        request = json_object_new_object();
        if (request == NULL) {
            safuLogErr("Failed to create request json object!");
        } else {
            json_object *jsonFilters;

            jsonFilters = safuJsonAddNewArray(request, "filter");
            if (jsonFilters == NULL) {
                safuLogErr("Failed to add filter array to message");
            } else {
                success = true;

                for (size_t i = 0; i < filterRuleArraySize; i++) {
                    if (filterRuleArray[i] == NULL) {
                        safuLogErr("Filter string can't be a NULL pointer!");
                        continue;
                    }
                    if (safuJsonAddNewString(jsonFilters, NULL, filterRuleArray[i]) == NULL) {
                        safuLogErr("Failed to add filter string to message");
                        success = false;
                        break;
                    }
                }

                if (success == true) {
                    uint8_t const requestId = ELOS_MESSAGE_EVENT_SUBSCRIBE;
                    json_object *response = NULL;
                    safuResultE_t retResult;

                    retResult = elosSendAndReceiveJsonMessage(session, requestId, request, &response);
                    if (retResult != SAFU_RESULT_OK) {
                        safuLogErr("Communication with elosd failed!");
                    } else {
                        json_object *eventQueueIdObj;
                        size_t idCount = 0;

                        eventQueueIdObj = safuJsonGetArray(response, "eventQueueIds", 0, &idCount);
                        if (eventQueueIdObj == NULL) {
                            safuLogErr("Failed to access response field 'eventQueueIds'");
                        } else if (idCount == 0) {
                            safuLogErr("Response field 'eventQueueIds' has no elements");
                        } else {
                            elosEventQueueId_t newEventQueueId = ELOS_ID_INVALID;
                            int retInt;

                            if (idCount > 1) {
                                safuLogWarn("Received more than one eventQueueId (not yet supported by libelos)");
                            }

                            retInt = safuJsonGetUint32(eventQueueIdObj, NULL, 0, &newEventQueueId);
                            if (retInt < 0) {
                                safuLogErr("Failed to read from field 'eventQueueIds'");
                            } else if (newEventQueueId == ELOS_ID_INVALID) {
                                safuLogErr("Received EventQueueId is invalid");
                            } else {
                                *eventQueueId = newEventQueueId;
                                result = SAFU_RESULT_OK;
                            }
                        }

                        json_object_put(response);
                    }
                }
            }

            json_object_put(request);
        }
    }

    return result;
}

safuResultE_t elosEventQueueRead(elosSession_t *session, elosEventQueueId_t eventQueueId,
                                 elosEventVector_t **eventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool retBool;

    retBool = elosSessionValid(session);
    if (retBool == false) {
        safuLogErr("Invalid session");
    } else if (eventVector == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        json_object *jRequest;

        jRequest = json_object_new_object();
        if (jRequest == NULL) {
            safuLogErr("Failed to allocate memory for json object");
        } else {
            json_object *retObj;

            retObj = safuJsonAddNewUint64(jRequest, "eventQueueId", eventQueueId);
            if (retObj == NULL) {
                safuLogErr("Failed to add eventQueueId to json object");
            } else {
                uint8_t const requestId = ELOS_MESSAGE_EVENTQUEUE_READ;
                safuResultE_t retResult;
                json_object *jResponse;

                retResult = elosSendAndReceiveJsonMessage(session, requestId, jRequest, &jResponse);
                if (retResult != SAFU_RESULT_OK) {
                    safuLogErr("Communication with elosd failed!");
                } else {
                    elosEventVector_t *newEventVector = NULL;
                    json_object *jEventArray;

                    jEventArray = safuJsonGetArray(jResponse, "eventArray", 0, NULL);
                    if (jEventArray == NULL) {
                        safuLogErr("Failed to access the 'eventArray' field from the elosd response");
                    } else {
                        retResult = elosEventVectorFromJsonArray(jEventArray, &newEventVector);
                        if (retResult == SAFU_RESULT_FAILED) {
                            safuLogErr("Failed to convert the 'eventArray' field from the elosd response");
                        } else {
                            *eventVector = newEventVector;
                            result = SAFU_RESULT_OK;
                        }
                    }

                    json_object_put(jResponse);
                }
            }

            json_object_put(jRequest);
        }
    }

    return result;
}
