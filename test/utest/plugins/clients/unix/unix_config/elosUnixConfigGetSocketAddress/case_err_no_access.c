// SPDX-License-Identifier: MIT

#include <errno.h>
#include <mock_unix_config.h>
#include <safu/result.h>
#include <sys/un.h>

#include "elosUnixConfigGetSocketAddress_utest.h"

int elosTestelosUnixConfigGetSocketAddressErrNoAccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixConfigGetSocketAddressErrNoAccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixConfigGetSocketAddressErrNoAccess(UNUSED void **state) {
    TEST("elosTestelosUnixConfigGetSocketAddressErrNoAccess");
    SHOULD("%s", "not get the socket address if access fails");

    elosPlugin_t plugin = {0};
    const char mockPath[] = MOCK_PATH;
    struct sockaddr_un addr = {0};

    MOCK_FUNC_AFTER_CALL(elosUnixConfigGetPath, 0);
    expect_value(elosUnixConfigGetPath, plugin, &plugin);
    will_return(elosUnixConfigGetPath, &mockPath);

    MOCK_FUNC_AFTER_CALL(access, 0);
    expect_value(__wrap_access, pathname, &mockPath);
    expect_any(__wrap_access, mode);
    will_return(__wrap_access, 1);

    MOCK_FUNC_WRAP(access_errno) = EPERM;

    safuResultE_t result = elosUnixConfigGetSocketAddress(&plugin, &addr);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_string_not_equal(addr.sun_path, MOCK_PATH);
    assert_int_not_equal(addr.sun_family, AF_UNIX);
}
