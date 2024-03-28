// SPDX-License-Identifier: MIT

#include "elos/libelosdlt/libelosdlt.h"

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "elos/libelosdlt/types.h"

#define ELOS_DLT_DEBUG 0

static long long _getSystemUptimeInNs(void) {
    struct timespec ts;
    clock_gettime(CLOCK_BOOTTIME, &ts);
    return ((long long)ts.tv_sec * 1000000000LL) + ts.tv_nsec;
}

safuResultE_t elosDltConnect(elosDltConnection_t *dlt, elosDltConnectionParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dlt == NULL || param == NULL) {
        result = SAFU_RESULT_FAILED;
    } else {
        if (dlt->pipePath != NULL) {
            result = elosDltConnectPipe(dlt);
        } else if (dlt->socketPath != NULL) {
            result = elosDltConnectUnix(dlt);
        } else if (dlt->host != NULL) {
            result = elosDltConnectTcp(dlt);
        }
    }
    return result;
}

safuResultE_t elosDltConnectUnix(elosDltConnection_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    int socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketFd == -1) {
        safuLogErrErrnoValue("socket set up failed", socketFd);
    } else {
        struct sockaddr_un address;
        memset(&address, 0, sizeof(struct sockaddr_un));
        address.sun_family = AF_UNIX;
        strncpy(address.sun_path, dlt->socketPath, sizeof(address.sun_path) - 1);
        int ret = connect(socketFd, (struct sockaddr *)&address, sizeof(struct sockaddr_un));
        if (ret == -1) {
            safuLogErrF("connect to path : %s failed!", dlt->socketPath);
            close(socketFd);
        } else {
            dlt->socketFd = socketFd;
            result = SAFU_RESULT_OK;
        }
    }
    return result;
}

safuResultE_t elosDltConnectTcp(elosDltConnection_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    struct addrinfo *addressList = NULL;
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
    };

    char port[6];
    snprintf(port, sizeof(port), "%d", dlt->port);
    int ret = getaddrinfo(dlt->host, port, &hints, &addressList);
    if (ret != 0) {
        safuLogErrF("Failed to fetch address info: %s", gai_strerror(ret));
    } else {
        for (struct addrinfo *endpoint = addressList; endpoint != NULL; endpoint = endpoint->ai_next) {
            int socketFd = socket(endpoint->ai_family, endpoint->ai_socktype, endpoint->ai_protocol);
            if (socketFd != -1) {
                ret = connect(socketFd, endpoint->ai_addr, endpoint->ai_addrlen);
                if (ret == -1) {
                    close(socketFd);
                } else {
                    dlt->socketFd = socketFd;
                    result = SAFU_RESULT_OK;
                    break;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosDltConnectPipe(elosDltConnection_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    int socketFd = open(dlt->pipePath, O_WRONLY | O_NONBLOCK | O_CLOEXEC);
    if (socketFd < 0) {
        safuLogErrErrnoValue("connect failed", socketFd);
    } else {
        dlt->socketFd = socketFd;
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosDltDisconnect(elosDltConnection_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    int ret = close(dlt->socketFd);
    if (ret != 0) {
        safuLogErrValue("close dlt connection failed", ret);
    } else {
        result = SAFU_RESULT_OK;
    }

    return result;
}

#if ELOS_DLT_DEBUG
void elosDltMessagePrint(const elosDltMessage_t *msg) {
    uint8_t *msgData = (uint8_t *)msg;

    safuLogDebugF("header type : %b (%.2x)\n", msgData[0], msgData[0]);
    safuLogDebugF("message count : %.2x\n", msgData[1]);
    safuLogDebugF("length : %.2x%.2x - %.2x\n", msgData[2], msgData[3], ntohs(msg->length));
    safuLogDebugF("message info : %.2x %.2x %b\n", ((msg->messageInfo & 0xE) >> 1), ((msg->messageInfo & 0x70) >> 4),
                  (msg->messageInfo & 0x1));
    safuLogDebugF("arg count : %d\n", msgData[offsetof(elosDltMessage_t, argCount)]);
    safuLogDebugF("appId : %.*s\n", DLT_ID_SIZE, &msgData[offsetof(elosDltMessage_t, appId)]);
    safuLogDebugF("contextId : %.*s\n", DLT_ID_SIZE, &msgData[offsetof(elosDltMessage_t, contextId)]);
    safuLogDebugF("ecuId : %.*s\n", DLT_ID_SIZE, &msgData[offsetof(elosDltMessage_t, ecuId)]);

    char msgHeaderAsHex[sizeof(elosDltMessage_t) * 3 + 1] = {0};
    for (size_t i = 0; i < sizeof(elosDltMessage_t); i++) {
        char element[sizeof("xx ")] = {0};
        sprintf(element, "%.2x ", ((uint8_t *)msg)[i]);
        strcat(&(msgHeaderAsHex[i * 3]), element);
    }
    safuLogDebugF("raw message (%lu bytes): %s\n", sizeof(elosDltMessage_t), msgHeaderAsHex);
}
#endif

safuResultE_t elosDltSendControlMessage(elosDltConnection_t *dlt, const char *payload, size_t payloadLength) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t msgLength = sizeof(elosDltMessage_t) + payloadLength + sizeof(elosDltControlMessage_t);
    elosDltMessage_t msg = {
        .headerTyp.useExtendedHeader = 1,
        .headerTyp.msbFirst = __BYTE_ORDER == __BIG_ENDIAN ? 1 : 0,
        .headerTyp.withEcuId = 1,
        .headerTyp.withSessionID = 1,
        .headerTyp.withTimestamp = 1,
        .headerTyp.protocolVersion1 = 1,

        .messageCount = ++dlt->messageCount,
        .monotonicTimestamp = _getSystemUptimeInNs(),
        .messageInfo = 0x03 << 1 | 0x01 << 4 | 0x1,
        .length = htons(msgLength),
        .argCount = 1,
    };
    memcpy(msg.ecuId, dlt->ecuId, DLT_ID_SIZE);
    memcpy(msg.appId, dlt->appId, DLT_ID_SIZE);
    memcpy(msg.contextId, dlt->contextId, DLT_ID_SIZE);

#if ELOS_DLT_DEBUG
    elosDltMessagePrint(&msg);
#endif

    ssize_t ret = safuWriteExactly(dlt->socketFd, &msg, sizeof(elosDltMessage_t));
    if (ret < 0) {
        safuLogErr("failed to send DLT log header");
    } else {
        elosDltControlMessage_t msgFrame = {
            .serviceID = 0xFFF,
            .length = payloadLength,
        };

        ret = safuWriteExactly(dlt->socketFd, (uint8_t *)&msgFrame, sizeof(msgFrame));
        if (ret < 0) {
            safuLogErr("failed to send DLT log header");
        } else {
            ret = safuWriteExactly(dlt->socketFd, payload, payloadLength);
            if (ret < 0) {
                safuLogErr("failed to send DLT log header");
            } else {
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}

safuResultE_t elosDltRegisterContext(elosDltConnection_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    ssize_t ret = 0;

    const char appDescription[] = "elos – the event logging system";
    elosDltRegisterApplication_t appMsg = {
        .header.magicNumber = {'D', 'U', 'H', 0x01},
        .header.type = ELOS_USER_MESSAGE_TYPE_REGISTER_APPLICATION,
        .pid = getpid(),
        .length = sizeof(appDescription),
    };
    memcpy(appMsg.appId, dlt->appId, DLT_ID_SIZE);

    ret = safuWriteExactly(dlt->socketFd, &appMsg, sizeof(appMsg));
    if (ret == -1) {
        safuLogErr("failed to send DLT message");
    } else {
        ret = safuWriteExactly(dlt->socketFd, appDescription, appMsg.length);
        if (ret == -1) {
            safuLogErr("failed to send DLT message");
        } else {
            const char contextDescription[] = "elos – storage plugin 1";
            elosDltRegisterContext_t contextMsg = {
                .header.magicNumber = {'D', 'U', 'H', 0x01},
                .header.type = ELOS_USER_MESSAGE_TYPE_REGISTER_CONTEXT,
                .pid = getpid(),
                .logLevel = 1,
                .traceStatus = 1,
                .length = sizeof(contextDescription),
            };
            memcpy(contextMsg.appId, dlt->appId, DLT_ID_SIZE);
            memcpy(contextMsg.contextId, dlt->contextId, DLT_ID_SIZE);

            ret = safuWriteExactly(dlt->socketFd, &contextMsg, sizeof(contextMsg));
            if (ret == -1) {
                safuLogErr("failed to send DLT message");
            } else {
                ret = safuWriteExactly(dlt->socketFd, contextDescription, contextMsg.length);
                if (ret == -1) {
                    safuLogErr("failed to send DLT message");
                } else {
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }
    return result;
}

safuResultE_t elosDltUnregisterContext(elosDltConnection_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    ssize_t ret = 0;

    elosDltUnregisterContext_t contextMsg = {
        .header.magicNumber = {'D', 'U', 'H', 0x01},
        .header.type = ELOS_USER_MESSAGE_TYPE_UNREGISTER_CONTEXT,
        .pid = getpid(),
    };
    memcpy(contextMsg.appId, dlt->appId, DLT_ID_SIZE);
    memcpy(contextMsg.contextId, dlt->contextId, DLT_ID_SIZE);

    ret = safuWriteExactly(dlt->socketFd, &contextMsg, sizeof(contextMsg));
    if (ret == -1) {
        safuLogErr("failed to send DLT message");
    } else {
        elosDltUnregisterApplication_t appMsg = {
            .header.magicNumber = {'D', 'U', 'H', 0x01},
            .header.type = ELOS_USER_MESSAGE_TYPE_UNREGISTER_APPLICATION,
            .pid = getpid(),
        };
        memcpy(appMsg.appId, dlt->appId, DLT_ID_SIZE);

        ret = safuWriteExactly(dlt->socketFd, &appMsg, sizeof(appMsg));
        if (ret == -1) {
            safuLogErr("failed to send DLT message");
        } else {
            result = SAFU_RESULT_OK;
        }
    }
    return result;
}

safuResultE_t elosDltSendUserLog(elosDltConnection_t *dlt, char *payload) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    size_t length = strlen(payload) + 1;
    size_t bufferSize = sizeof(elosDltUserMessage_t) + sizeof(elosDltMessage_t) + sizeof(elosDltLogArgument_t) + length;

    elosDltUserMessage_t userHeader = {
        .magicNumber = {'D', 'U', 'H', 0x01},
        .type = ELOS_USER_MESSAGE_TYPE_LOG,
    };

    elosDltMessage_t msgHeader = {
        .headerTyp.useExtendedHeader = 1,
        .headerTyp.msbFirst = __BYTE_ORDER == __BIG_ENDIAN ? 1 : 0,
        .headerTyp.withEcuId = 1,
        .headerTyp.withSessionID = 1,
        .headerTyp.withTimestamp = 1,
        .headerTyp.protocolVersion1 = 1,

        .messageCount = ++dlt->messageCount,
        .monotonicTimestamp = _getSystemUptimeInNs(),
        .messageInfo = 0x00 << 1 | 0x01 << 4 | 0x1,
        .length = htons(bufferSize - sizeof(elosDltUserMessage_t)),
        .argCount = 1,
    };
    memcpy(msgHeader.ecuId, dlt->ecuId, DLT_ID_SIZE);
    memcpy(msgHeader.appId, dlt->appId, DLT_ID_SIZE);
    memcpy(msgHeader.contextId, dlt->contextId, DLT_ID_SIZE);

    elosDltLogArgument_t logString = {
        .typeInfo = 0x200 | 0x800,
        .argSize = length,
        .nameSize = 20,
        .name = "MyVariableName16181",
    };

#if ELOS_DLT_DEBUG
    elosDltMessagePrint(&msgHeader);
#endif

    uint8_t *buffer = calloc(1, bufferSize);
    if (buffer != NULL) {
        uint8_t *pos = buffer;

        memcpy(pos, (uint8_t *)&userHeader, sizeof(elosDltUserMessage_t));
        pos += sizeof(elosDltUserMessage_t);
        memcpy(pos, (uint8_t *)&msgHeader, sizeof(elosDltMessage_t));
        pos += sizeof(elosDltMessage_t);
        memcpy(pos, (uint8_t *)&logString, sizeof(elosDltLogArgument_t));
        pos += sizeof(elosDltLogArgument_t);
        memcpy(pos, (uint8_t *)payload, logString.argSize);

        ssize_t ret = safuWriteExactly(dlt->socketFd, buffer, bufferSize);
        if (ret == -1) {
            safuLogErr("failed to send DLT message");
        } else {
            result = SAFU_RESULT_OK;
        }

        free(buffer);
    }

    return result;
}
