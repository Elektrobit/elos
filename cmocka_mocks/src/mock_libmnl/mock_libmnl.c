// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_libmnl.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(mnl_socket_open, struct mnl_socket *, int bus) {
    if (MOCK_IS_ACTIVE(mnl_socket_open)) {
        check_expected(bus);
        return mock_ptr_type(struct mnl_socket *);
    }
    return MOCK_FUNC_REAL(mnl_socket_open)(bus);
}

MOCK_FUNC_BODY(mnl_socket_bind, int, struct mnl_socket *nl, unsigned int groups, pid_t pid) {
    if (MOCK_IS_ACTIVE(mnl_socket_bind)) {
        check_expected_ptr(nl);
        check_expected(groups);
        check_expected(pid);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(mnl_socket_bind)(nl, groups, pid);
}

MOCK_FUNC_BODY(mnl_socket_close, int, struct mnl_socket *nl) {
    if (MOCK_IS_ACTIVE(mnl_socket_close)) {
        check_expected_ptr(nl);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(mnl_socket_close)(nl);
}

MOCK_FUNC_BODY(mnl_socket_recvfrom, ssize_t, const struct mnl_socket *nl, void *buf, size_t siz) {
    if (MOCK_IS_ACTIVE(mnl_socket_recvfrom)) {
        check_expected_ptr(nl);
        check_expected_ptr(buf);
        check_expected(siz);
        return mock_type(ssize_t);
    }
    return MOCK_FUNC_REAL(mnl_socket_recvfrom)(nl, buf, siz);
}

MOCK_FUNC_BODY(mnl_socket_sendto, ssize_t, const struct mnl_socket *nl, const void *req, size_t siz) {
    if (MOCK_IS_ACTIVE(mnl_socket_sendto)) {
        check_expected_ptr(nl);
        check_expected_ptr(req);
        check_expected(siz);
        return mock_type(ssize_t);
    }
    return MOCK_FUNC_REAL(mnl_socket_sendto)(nl, req, siz);
}

MOCK_FUNC_BODY(mnl_socket_get_portid, unsigned int, struct mnl_socket *nl) {
    if (MOCK_IS_ACTIVE(mnl_socket_get_portid)) {
        check_expected_ptr(nl);
        return mock_type(unsigned int);
    }
    return MOCK_FUNC_REAL(mnl_socket_get_portid)(nl);
}
