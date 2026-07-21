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

struct unixConnectResults unixConnectRes;

int __wrap_socket(int domain, int type, int protocol) {
    unixConnectRes.domain = domain;
    unixConnectRes.type = type;
    unixConnectRes.protocol = protocol;
    return 0;
}

int __wrap_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (addr->sa_family == AF_UNIX) {
        strncpy(unixConnectRes.addrUnix.sun_path, addr->sa_data, sizeof(unixConnectRes.addrUnix.sun_path));
    }
    return 0;
}

int elosliteConnectUnix_test_successSetup(UNUSED void **state) {
    return 0;
}

int elosliteConnectUnix_test_successTeardown(UNUSED void **state) {
    return 0;
}

void elosliteConnectUnix_test_success(UNUSED void **state) {
    const char *sockname = "/run/elosd/elosd.socket";
    elosliteSession_t session;

    assert_true(elosliteConnectUnix(sockname, &session));
    assert_string_equal(unixConnectRes.addrUnix.sun_path, sockname);
}
