// SPDX-License-Identifier: MIT

#include "elos/libelos_lite.h"

#include <arpa/inet.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/auxv.h>
#include <sys/socket.h>
#include <unistd.h>

#include "elos/lite/event.h"

const char *elosliteGetLibraryVersion() {
    return ELOS_VERSION;
}

static bool _connectTcpipv4(const char *host, uint16_t port, elosliteSession_t *session);
static bool _connectTcpipv6(const char *host, uint16_t port, elosliteSession_t *session);

bool elosliteConnect(struct addrinfo addrInfo, elosliteSession_t *session) {
    int sfd = socket(addrInfo.ai_family, addrInfo.ai_socktype, addrInfo.ai_protocol);
    if (sfd == -1) {
        return false;
    }
    int retVal = connect(sfd, addrInfo.ai_addr, addrInfo.ai_addrlen);
    if (retVal == -1) {
        close(sfd);
        return false;
    }
    session->fd = sfd;
    session->connected = true;
    return true;
}

bool elosliteConnectTcpip(const char *host, uint16_t port, elosliteSession_t *session) {
    if (_connectTcpipv4(host, port, session)) {
        return true;
    }
    if (_connectTcpipv6(host, port, session)) {
        return true;
    }
    return false;
}

bool _connectTcpipv4(const char *host, uint16_t port, elosliteSession_t *session) {
    struct in_addr inAddr = {0};
    if (!inet_aton(host, &inAddr)) {
        return false;
    }
    struct sockaddr_in sockaddrIn = {
        .sin_addr = inAddr,
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    struct addrinfo addinf = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
        .ai_addr = (struct sockaddr *)&sockaddrIn,
        .ai_addrlen = sizeof(sockaddrIn),
    };
    return elosliteConnect(addinf, session);
}

bool _connectTcpipv6(const char *host, uint16_t port, elosliteSession_t *session) {
    struct in6_addr inAddr = {0};
    if (!inet_pton(AF_INET6, host, &inAddr)) {
        return false;
    }
    struct sockaddr_in6 sockaddrIn = {
        .sin6_addr = inAddr,
        .sin6_family = AF_INET6,
        .sin6_port = htons(port),
    };
    struct addrinfo addinf = {
        .ai_family = AF_INET6,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
        .ai_addr = (struct sockaddr *)&sockaddrIn,
        .ai_addrlen = sizeof(sockaddrIn),
    };
    return elosliteConnect(addinf, session);
}

bool elosliteDisconnect(elosliteSession_t *session) {
    if (session == NULL) {
        return false;
    }
    int retVal = close(session->fd);
    if (retVal < 0) {
        return false;
    }
    session->connected = false;

    return true;
}

#define ELOS_PROTOCOL_VERSION               0x01
#define ELOS_MESSAGE_EVENT_PUBLISH          0x02
#define ELOS_MESSAGE_RESPONSE_BIT           0x80
#define ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH (ELOS_MESSAGE_EVENT_PUBLISH | ELOS_MESSAGE_RESPONSE_BIT)

struct elosliteMessageHead {
    uint8_t version;
    uint8_t message;
    uint16_t length;
};

static uint16_t _numLen(int64_t num) {
    uint16_t result = 0;
    if (num == 0) result = 1;
    while (num) {
        num /= 10;
        result += 1;
    }
    return result;
}

static uint16_t _msgLen(elosliteEvent_t *event) {
    uint16_t msgLen = 2;
    bool elementBefore = false;
    if (event->date.tv_sec != 0 || event->date.tv_nsec != 0) {
        elementBefore = true;
        msgLen += strlen("\"date\":[,]");
        msgLen += _numLen(event->date.tv_sec);
        msgLen += _numLen(event->date.tv_nsec);
    }
    if (event->source.appName != NULL || event->source.fileName != NULL || event->source.pid != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"source\":{}");
        bool sourceElementBefore = false;
        if (event->source.appName != NULL) {
            sourceElementBefore = true;
            msgLen += strlen("\"appName\":\"\"");
            msgLen += strlen(event->source.appName);
        }
        if (event->source.fileName != NULL) {
            msgLen += sourceElementBefore ? 1 : 0;
            sourceElementBefore = true;
            msgLen += strlen("\"fileName\":\"\"");
            msgLen += strlen(event->source.fileName);
        }
        if (event->source.pid != 0) {
            msgLen += sourceElementBefore ? 1 : 0;
            msgLen += strlen("\"pid\":");
            msgLen += _numLen(event->source.pid);
        }
    }
    if (event->severity != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"severity\":");
        msgLen += _numLen(event->severity);
    }
    if (event->hardwareid != NULL) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"hardwareid\":\"\"");
        msgLen += strlen(event->hardwareid);
    }
    if (event->classification != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"classification\":");
        msgLen += _numLen(event->classification);
    }
    if (event->messageCode != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"messageCode\":");
        msgLen += _numLen(event->messageCode);
    }
    if (event->payload != NULL) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"payload\":\"\"");
        msgLen += strlen(event->payload);
    }
    return msgLen;
}

/*******************************************************************
 * holds a small buffeer for sending events
 ******************************************************************/
typedef struct elosliteBuffer {
    char *buffer;
    size_t size;
    size_t pos;
} elosliteBuffer_t;

#ifndef ELOSLITE_SEND_BUFFER
#define ELOSLITE_SEND_BUFFER 60 /* AT_PAGESZ */
#endif

static int _sendFlush(elosliteSession_t *session, elosliteBuffer_t *buffer) {
    int sendNum = send(session->fd, (void *)buffer->buffer, buffer->pos, MSG_NOSIGNAL);
    buffer->pos = 0;
    buffer->buffer[0] = 0;
    return sendNum;
}

static int _sendPartF(elosliteSession_t *session, elosliteBuffer_t *buffer, const char *fmt, ...) {
    int sendNum = 0;
    int sizeLeft = buffer->size - buffer->pos;
    va_list args;
    va_start(args, fmt);
    int off = vsnprintf(&buffer->buffer[buffer->pos], sizeLeft, fmt, args);
    va_end(args);
    if (off < 0) {
        return -1;
    } else if (off >= sizeLeft) {
        buffer->buffer[buffer->pos] = 0;
        sendNum = _sendFlush(session, buffer);
        va_list rargs;
        va_start(rargs, fmt);
        off = vsnprintf(buffer->buffer, buffer->size, fmt, rargs);
        va_end(rargs);
        if (off < 0 || (size_t)off > buffer->size) {
            return -1;
        } else {
            buffer->pos += off;
        }
    } else {
        buffer->pos += off;
    }
    return sendNum;
}

static int _sendPartStr(elosliteSession_t *session, elosliteBuffer_t *buffer, const char *str) {
    int sendNum = 0;
    size_t strLen = strlen(str);
    if (strLen < buffer->size) {
        sendNum = _sendPartF(session, buffer, str);
    } else {
        sendNum = _sendFlush(session, buffer);
        if (sendNum >= 0) {
            int sendStrLen = send(session->fd, (void *)str, strLen, MSG_NOSIGNAL);
            if (sendStrLen < 0) {
                return sendStrLen;
            }
            sendNum += sendStrLen;
        }
    }
    return sendNum;
}

static int _sendMsgParts(elosliteSession_t *session, elosliteEvent_t *event) {
    char partBuf[ELOSLITE_SEND_BUFFER];
    elosliteBuffer_t buffer = {
        .buffer = partBuf,
        .size = ELOSLITE_SEND_BUFFER,
        .pos = 0,
    };
    size_t sendNum = 0;
    sendNum += _sendPartF(session, &buffer, "{");
    bool elementBefore = false;
    if (event->date.tv_sec != 0 || event->date.tv_nsec != 0) {
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"date\":[%ld,%ld]", event->date.tv_sec, event->date.tv_nsec);
    }
    if (event->source.appName != NULL || event->source.fileName != NULL || event->source.pid != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"source\":{");
        bool sourceElementBefore = false;
        if (event->source.appName != NULL) {
            sourceElementBefore = true;
            sendNum += _sendPartF(session, &buffer, "\"appName\":\"");
            sendNum += _sendPartStr(session, &buffer, event->source.appName);
            sendNum += _sendPartF(session, &buffer, "\"");
        }
        if (event->source.fileName != NULL) {
            if (sourceElementBefore) {
                sendNum += _sendPartF(session, &buffer, ",");
            }
            sourceElementBefore = true;
            sendNum += _sendPartF(session, &buffer, "\"fileName\":\"");
            sendNum += _sendPartStr(session, &buffer, event->source.fileName);
            sendNum += _sendPartF(session, &buffer, "\"");
        }
        if (event->source.pid != 0) {
            if (sourceElementBefore) {
                sendNum += _sendPartF(session, &buffer, ",");
            }
            sendNum += _sendPartF(session, &buffer, "\"pid\":%d", event->source.pid);
        }
        sendNum += _sendPartF(session, &buffer, "}", event->source.pid);
    }
    if (event->severity != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"severity\":%d", event->severity);
    }
    if (event->hardwareid != NULL) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"hardwareid\":\"");
        sendNum += _sendPartStr(session, &buffer, event->hardwareid);
        sendNum += _sendPartF(session, &buffer, "\"");
    }
    if (event->classification != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"classification\":%ld", event->classification);
    }
    if (event->messageCode != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"messageCode\":%d", event->messageCode);
    }
    if (event->payload != NULL) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"payload\":\"");
        sendNum += _sendPartStr(session, &buffer, event->payload);
        sendNum += _sendPartF(session, &buffer, "\"");
    }
    sendNum += _sendPartF(session, &buffer, "}");
    sendNum += _sendFlush(session, &buffer);
    return sendNum;
}

static bool _msgHandleResponse(elosliteSession_t *session) {
    struct elosliteMessageHead msgHead = {0};
    recv(session->fd, &msgHead, sizeof(msgHead), 0);
    if ((msgHead.message != ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH) || (msgHead.version != ELOS_PROTOCOL_VERSION)) {
        return false;
    }
    char buf[20];
    uint16_t lengthRecv = 0;
    while (lengthRecv < msgHead.length) {
        int res = recv(session->fd, buf, 20, 0);
        if (res < 0) {
            return false;
        }
        lengthRecv += res;
    }
    return true;
}

bool eloslitePublish(elosliteSession_t *session, elosliteEvent_t *event) {
    if (session == NULL || event == NULL || session->connected == false) {
        return false;
    }
    struct elosliteMessageHead msgHead = {
        .version = ELOS_PROTOCOL_VERSION,
        .message = ELOS_MESSAGE_EVENT_PUBLISH,
        .length = _msgLen(event),
    };
    int sendNum = send(session->fd, (void *)&msgHead, sizeof(msgHead), MSG_NOSIGNAL);
    if (sendNum < 0) {
        elosliteDisconnect(session);
        return false;
    }
    sendNum = _sendMsgParts(session, event);
    if (sendNum != msgHead.length) {
        elosliteDisconnect(session);
        return false;
    }
    if (!_msgHandleResponse(session)) {
        elosliteDisconnect(session);
        return false;
    }
    return true;
}
