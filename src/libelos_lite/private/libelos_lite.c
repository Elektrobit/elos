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

#ifndef ELOSLITE_SEND_BUFFER
#define ELOSLITE_SEND_BUFFER 60 /* AT_PAGESZ */
#endif

static int _sendFlush(elosliteSession_t *session) {
    printf("Buffer[%ld]: %s\n", strlen(session->buffer.buffer), session->buffer.buffer);
    int sendNum = send(session->fd, (void *)session->buffer.buffer, session->buffer.pos, MSG_NOSIGNAL);
    session->buffer.pos = 0;
    session->buffer.buffer[0] = 0;
    return sendNum;
}

static int _sendPartF(elosliteSession_t *session, const char *fmt, ...) {
    int sendNum = 0;
    int sizeLeft = session->buffer.size - session->buffer.pos;
    va_list args;
    va_start(args, fmt);
    int off = vsnprintf(&session->buffer.buffer[session->buffer.pos], sizeLeft, fmt, args);
    va_end(args);
    if (off < 0) {
        return -1;
    } else if (off >= sizeLeft) {
        session->buffer.buffer[session->buffer.pos] = 0;
        sendNum = _sendFlush(session);
        va_list rargs;
        va_start(rargs, fmt);
        off = vsnprintf(session->buffer.buffer, session->buffer.size, fmt, rargs);
        va_end(rargs);
        if (off < 0 || (size_t)off > session->buffer.size) {
            return -1;
        } else {
            session->buffer.pos += off;
        }
    } else {
        session->buffer.pos += off;
    }
    return sendNum;
}

static int _sendPartStr(elosliteSession_t *session, const char *str) {
    int sendNum = 0;
    size_t strLen = strlen(str);
    if (strLen < session->buffer.size) {
        sendNum = _sendPartF(session, str);
    } else {
        sendNum = _sendFlush(session);
        if (sendNum >= 0) {
            printf("Str[%ld]:  %s\n", strLen, str);
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
    session->buffer.buffer = partBuf;
    session->buffer.size = ELOSLITE_SEND_BUFFER;
    session->buffer.pos = 0;
    size_t sendNum = 0;
    sendNum += _sendPartF(session, "{");
    bool elementBefore = false;
    if (event->date.tv_sec != 0 || event->date.tv_nsec != 0) {
        elementBefore = true;
        sendNum += _sendPartF(session, "\"date\":[%ld,%ld]", event->date.tv_sec, event->date.tv_nsec);
    }
    if (event->source.appName != NULL || event->source.fileName != NULL || event->source.pid != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, "\"source\":{");
        bool sourceElementBefore = false;
        if (event->source.appName != NULL) {
            sourceElementBefore = true;
            sendNum += _sendPartF(session, "\"appName\":\"");
            sendNum += _sendPartStr(session, event->source.appName);
            sendNum += _sendPartF(session, "\"");
        }
        if (event->source.fileName != NULL) {
            if (sourceElementBefore) {
                sendNum += _sendPartF(session, ",");
            }
            sourceElementBefore = true;
            sendNum += _sendPartF(session, "\"fileName\":\"");
            sendNum += _sendPartStr(session, event->source.fileName);
            sendNum += _sendPartF(session, "\"");
        }
        if (event->source.pid != 0) {
            if (sourceElementBefore) {
                sendNum += _sendPartF(session, ",");
            }
            sendNum += _sendPartF(session, "\"pid\":%d", event->source.pid);
        }
        sendNum += _sendPartF(session, "}", event->source.pid);
    }
    if (event->severity != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, "\"severity\":%d", event->severity);
    }
    if (event->hardwareid != NULL) {
        if (elementBefore) {
            sendNum += _sendPartF(session, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, "\"hardwareid\":\"");
        sendNum += _sendPartStr(session, event->hardwareid);
        sendNum += _sendPartF(session, "\"");
    }
    if (event->classification != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, "\"classification\":%ld", event->classification);
    }
    if (event->messageCode != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, "\"messageCode\":%d", event->messageCode);
    }
    if (event->payload != NULL) {
        if (elementBefore) {
            sendNum += _sendPartF(session, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, "\"payload\":\"");
        sendNum += _sendPartStr(session, event->payload);
        sendNum += _sendPartF(session, "\"");
    }
    sendNum += _sendPartF(session, "}");
    sendNum += _sendFlush(session);
    return sendNum;
}

static bool _msgHandleResponse(elosliteSession_t *session) {
    struct elosliteMessageHead msgHead = {0};
    recv(session->fd, &msgHead, sizeof(msgHead), 0);
    if (msgHead.message != ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH) {
        return false;
    }
    char buf[20];
    while (msgHead.length > 0) {
        int res = recv(session->fd, buf, 50, 0);
        msgHead.length -= res;
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
    send(session->fd, (void *)&msgHead, sizeof(msgHead), MSG_NOSIGNAL);
    int sendNum = _sendMsgParts(session, event);
    if (sendNum != msgHead.length) {
        return false;
    }
    _msgHandleResponse(session);
    return true;
}
