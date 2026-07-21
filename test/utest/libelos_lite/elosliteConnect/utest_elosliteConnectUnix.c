// SPDX-License-Identifier: MIT

#include <elos/libelos_lite.h>
#include <sys/un.h>

#include "elosliteConnect_utest.h"

struct unixConnectResults {
    int domain;
    int type;
    int protocol;
    struct sockaddr_un addrUnix;
};

struct unixConnectResults elosliteUnixConnectRes;

int __wrap_socket(int domain, int type, int protocol) {
    elosliteUnixConnectRes.domain = domain;
    elosliteUnixConnectRes.type = type;
    elosliteUnixConnectRes.protocol = protocol;
    return 0;
}

int __wrap_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (addr->sa_family == AF_UNIX) {
        strncpy(elosliteUnixConnectRes.addrUnix.sun_path, addr->sa_data, sizeof(elosliteUnixConnectRes.addrUnix.sun_path));
    }
    return 0;
}

int elosliteConnectUnixTestSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosliteConnectUnixTestSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosliteConnectUnixTestSuccess(UNUSED void **state) {
    const char *sockname = "/run/elosd/elosd.socket";
    elosliteSession_t session;

    assert_true(elosliteConnectUnix(sockname, &session));
    assert_string_equal(elosliteUnixConnectRes.addrUnix.sun_path, sockname);
}
