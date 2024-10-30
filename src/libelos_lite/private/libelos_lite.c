// SPDX-License-Identifier: MIT

#include "elos/libelos_lite.h"
#include "elos/lite/event.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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
#define ELOS_PROTOCOL_VERSION         0x01
#define ELOS_MESSAGE_EVENT_PUBLISH    0x02

struct elosliteMessage {
    uint8_t version;
    uint8_t message;
    uint16_t length;
    char json[500];
};

struct elosliteMessageHead {
    uint8_t version;
    uint8_t message;
    uint16_t length;
};

static size_t _numLen(int64_t num) {
    //printf("#(%ld) = ", num);
    size_t result = 0;
    if (num == 0) result = 1;
    while (num) {
        num /= 10;
        result += 1;
    }
    //printf("%ld\n", result);
    return result; // length of number as base10 string
}
static size_t _msgLen(elosliteEvent_t *event) {
    size_t msgLen = 2;
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

static bool _prepMsg(elosliteEvent_t *event, struct elosliteMessage *msg) {
    msg->version = ELOS_PROTOCOL_VERSION;
    msg->message = ELOS_MESSAGE_EVENT_PUBLISH;

    size_t idx = 0;
    idx += sprintf(msg->json + idx, "{");
    bool elementBefore = false;
    if (event->date.tv_sec != 0 || event->date.tv_nsec != 0) {
        elementBefore = true;
        idx += sprintf(msg->json + idx, "\"date\":[%ld,%ld]", event->date.tv_sec, event->date.tv_nsec);
    }
    if (event->source.appName != NULL || event->source.fileName != NULL || event->source.pid != 0) {
        idx += sprintf(msg->json + idx, "%s", elementBefore ? "," : "");
        elementBefore = true;
        idx += sprintf(msg->json + idx, "\"source\":{");
        bool sourceElementBefore = false;
        if (event->source.appName != NULL) {
            sourceElementBefore = true;
            idx += sprintf(msg->json + idx, "\"appName\":\"%s\"", event->source.appName);
        }
        if (event->source.fileName != NULL) {
            idx += sprintf(msg->json + idx, "%s", sourceElementBefore ? "," : "");
            sourceElementBefore = true;
            idx += sprintf(msg->json + idx, "\"fileName\":\"%s\"", event->source.fileName);
        }
        if (event->source.pid != 0) {
            idx += sprintf(msg->json + idx, "%s", sourceElementBefore ? "," : "");
            idx += sprintf(msg->json + idx, "\"pid\":%d", event->source.pid);
        }
        idx += sprintf(msg->json + idx, "}");
    }
    if (event->severity != 0) {
        idx += sprintf(msg->json + idx, "%s", elementBefore ? "," : "");
        elementBefore = true;
        idx += sprintf(msg->json + idx, "\"severity\":%d", event->severity);
    }
    if (event->hardwareid != NULL) {
        idx += sprintf(msg->json + idx, "%s", elementBefore ? "," : "");
        elementBefore = true;
        idx += sprintf(msg->json + idx, "\"hardwareid\":\"%s\"", event->hardwareid);
    }
    if (event->classification != 0) {
        idx += sprintf(msg->json + idx, "%s", elementBefore ? "," : "");
        elementBefore = true;
        idx += sprintf(msg->json + idx, "\"classification\":%ld", event->classification);
    }
    if (event->messageCode != 0) {
        idx += sprintf(msg->json + idx, "%s", elementBefore ? "," : "");
        elementBefore = true;
        idx += sprintf(msg->json + idx, "\"messageCode\":%d", event->messageCode);
    }
    if (event->payload != NULL) {
        idx += sprintf(msg->json + idx, "%s", elementBefore ? "," : "");
        elementBefore = true;
        idx += sprintf(msg->json + idx, "\"payload\":\"%s\"", event->payload);
    }
    idx += sprintf(msg->json + idx, "}");
    msg->length = strlen(msg->json);
    return true;
}

static bool _sendMsgParts(elosliteSession_t *session, elosliteEvent_t *event) {
    char partBuf[60];
    size_t sendNum = 0;
    sendNum += send(session->fd, (void *)"{", 1, MSG_NOSIGNAL);
    bool elementBefore = false;
    if (event->date.tv_sec != 0 || event->date.tv_nsec != 0) {
        elementBefore = true;
        sprintf(partBuf, "\"date\":[%ld,%ld]", event->date.tv_sec, event->date.tv_nsec); // maxlen: 10 + 20 + 20 + 1 = 51
        sendNum += send(session->fd, (void *)partBuf, strlen(partBuf), MSG_NOSIGNAL);
    }
    if (event->source.appName != NULL || event->source.fileName != NULL || event->source.pid != 0) {
        if (elementBefore) {
            sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
        }
        elementBefore = true;
        char *part = "\"source\":{";
        sendNum += send(session->fd, (void *)part, strlen(part), MSG_NOSIGNAL);
        bool sourceElementBefore = false;
        if (event->source.appName != NULL) {
            sourceElementBefore = true;
            part = "\"appName\":\"";
            sendNum += send(session->fd, (void *)part, strlen(part), MSG_NOSIGNAL);
            sendNum += send(session->fd, (void *)event->source.appName, strlen(event->source.appName), MSG_NOSIGNAL);
            sendNum += send(session->fd, (void *)"\"", 1, MSG_NOSIGNAL);
        }
        if (event->source.fileName != NULL) {
            if (sourceElementBefore) {
                sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
            }
            sourceElementBefore = true;
            part = "\"fileName\":\"";
            sendNum += send(session->fd, (void *)part, strlen(part), MSG_NOSIGNAL);
            sendNum += send(session->fd, (void *)event->source.fileName, strlen(event->source.fileName), MSG_NOSIGNAL);
            sendNum += send(session->fd, (void *)"\"", 1, MSG_NOSIGNAL);
        }
        if (event->source.pid != 0) {
            if (sourceElementBefore) {
                sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
            }
            sprintf(partBuf, "\"pid\":%d", event->source.pid); // maxlen: 6 + 10 + 1 = 17
            sendNum += send(session->fd, (void *)partBuf, strlen(partBuf), MSG_NOSIGNAL);
        }
        sendNum += send(session->fd, (void *)&"}", 1, MSG_NOSIGNAL);
    }
    if (event->severity != 0) {
        if (elementBefore) {
            sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
        }
        elementBefore = true;
        sprintf(partBuf, "\"severity\":%d", event->severity); // maxlen: 10 + 10 + 1 = 21
        sendNum += send(session->fd, (void *)partBuf, strlen(partBuf), MSG_NOSIGNAL);
    }
    if (event->hardwareid != NULL) {
        if (elementBefore) {
            sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
        }
        elementBefore = true;
        char *part = "\"hardwareid\":\"";
        sendNum += send(session->fd, (void *)part, strlen(part), MSG_NOSIGNAL);
        sendNum += send(session->fd, (void *)event->hardwareid, strlen(event->hardwareid), MSG_NOSIGNAL);
        sendNum += send(session->fd, (void *)"\"", 1, MSG_NOSIGNAL);
    }
    if (event->classification != 0) {
        if (elementBefore) {
            sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
        }
        elementBefore = true;
        sprintf(partBuf, "\"classification\":%ld", event->classification); // maxlen: 10 + 20 + 1 = 31
        sendNum += send(session->fd, (void *)partBuf, strlen(partBuf), MSG_NOSIGNAL);
    }
    if (event->messageCode != 0) {
        if (elementBefore) {
            sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
        }
        elementBefore = true;
        sprintf(partBuf, "\"messageCode\":%d", event->messageCode); // maxlen: 10 + 10 + 1 = 21
        sendNum += send(session->fd, (void *)partBuf, strlen(partBuf), MSG_NOSIGNAL);
    }
    if (event->payload != NULL) {
        if (elementBefore) {
            sendNum += send(session->fd, (void *)",", 1, MSG_NOSIGNAL);
        }
        elementBefore = true;
        char *part = "\"payload\":\"";
        sendNum += send(session->fd, (void *)part, strlen(part), MSG_NOSIGNAL);
        sendNum += send(session->fd, (void *)event->payload, strlen(event->payload), MSG_NOSIGNAL);
        sendNum += send(session->fd, (void *)"\"", 1, MSG_NOSIGNAL);
    }
    sendNum += send(session->fd, (void *)"}", 1, MSG_NOSIGNAL);
    printf("sendNum: %ld\n", sendNum);
    return true;
}
bool eloslitePublish(elosliteSession_t *session, elosliteEvent_t *event) {
    if (session == NULL || event == NULL || session->connected == false) {
        return false;
    }
    //struct elosliteMessage msg = {0};
    //_prepMsg(event, &msg);
    //send(session->fd, (void *)&msg, sizeof(msg), MSG_NOSIGNAL);
    //printf("%s\n", msg.json);

    struct elosliteMessageHead msgHead = {
        .version = ELOS_PROTOCOL_VERSION,
        .message = ELOS_MESSAGE_EVENT_PUBLISH,
        .length = _msgLen(event),
    };
    send(session->fd, (void *)&msgHead, sizeof(msgHead), MSG_NOSIGNAL);
    printf("msgLen: %d\n", msgHead.length);
    _sendMsgParts(session, event);

    return true;
}
