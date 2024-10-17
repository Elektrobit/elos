// SPDX-License-Identifier: MIT

#include "elos/libelos_lite.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

const char *elosGetLibraryVersion() {
    return ELOS_VERSION;
}

static bool _connectTcpipv4(const char *host, uint16_t port, elosSession_t *session);
static bool _connectTcpipv6(const char *host, uint16_t port, elosSession_t *session);

bool elosConnect(struct addrinfo addrInfo, elosSession_t *session) {
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

bool elosConnectTcpip(const char *host, uint16_t port, elosSession_t *session) {
    if (_connectTcpipv4(host, port, session)) {
        return true;
    }
    if (_connectTcpipv6(host, port, session)) {
        return true;
    }
    return false;
}

bool _connectTcpipv4(const char *host, uint16_t port, elosSession_t *session) {
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
    return elosConnect(addinf, session);
}

bool _connectTcpipv6(const char *host, uint16_t port, elosSession_t *session) {
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
    return elosConnect(addinf, session);
}

bool elosDisconnect(elosSession_t *session) {
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
