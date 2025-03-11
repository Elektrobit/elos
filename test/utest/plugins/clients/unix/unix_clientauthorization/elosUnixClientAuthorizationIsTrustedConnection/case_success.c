// SPDX-License-Identifier: MIT
#define _GNU_SOURCE
#include <sys/socket.h>
#include <unix_clientauthorization/clientauthorization.h>

#include "elosUnixClientAuthorizationIsTrustedConnection_utest.h"

int elosTestelosUnixClientAuthorizationIsTrustedConnectionSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixClientAuthorizationIsTrustedConnectionSuccessTeardown(UNUSED void **state) {
    MOCK_FUNC_NEVER(getsockopt);
    MOCK_FUNC_NEVER(getsockopt);
    return 0;
}

int _customGetsockopt(int fd, int level, int optname, void *optval, UNUSED socklen_t *optlen) {
    assert_int_equal(fd, MOCK_FD);
    assert_int_equal(level, SOL_SOCKET);
    assert_int_equal(optname, SO_PEERCRED);

    struct ucred *clientCredentials = (struct ucred *)optval;
    clientCredentials->pid = MOCK_PID;

    return 1;
}

int _customReadlink(const char *pathname, char *buf, size_t bufsiz) {
    assert_string_equal(pathname, MOCK_PATH);
    assert_int_equal(bufsiz, PATH_MAX - 1);

    size_t len = strlen(MOCK_PATH);

    memcpy(buf, MOCK_PATH, len);

    return (int)len;
}

void elosTestelosUnixClientAuthorizationIsTrustedConnectionSuccess(UNUSED void **state) {
    bool result = false;

    TEST("elosUnixClientAuthorizationIsTrustedConnection");
    SHOULD("%s", "successfully do a client authorization lookup");

    elosClientAuthorization_t clientAuth = {0};

    MOCK_FUNC_ALWAYS_WITH(getsockopt, _customGetsockopt, NULL);

    MOCK_FUNC_ALWAYS_WITH(readlink, _customReadlink, NULL);

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_string(__wrap_strdup, string, MOCK_PATH);
    will_return(__wrap_strdup, MOCK_ADDRESS);

    MOCK_FUNC_AFTER_CALL(free, 0);
    expect_value(__wrap_free, ptr, MOCK_ADDRESS);

    MOCK_FUNC_AFTER_CALL(elosAuthorizedProcessCheck, 0)
    expect_value(elosAuthorizedProcessCheck, authorizedprocesses, &clientAuth.authorizedProcessFilters);
    expect_any(elosAuthorizedProcessCheck, process);
    will_return(elosAuthorizedProcessCheck, true);

    result = elosUnixClientAuthorizationIsTrustedConnection(&clientAuth, MOCK_FD);
    assert_int_equal(result, true);
}
