// SPDX-License-Identifier: MIT

#include <errno.h>
#include <mock_unix_config.h>
#include <safu/result.h>
#include <sys/un.h>

#include "elosUnixConfigGetSocketAddress_utest.h"

int elosTestelosUnixConfigGetSocketAddressSuccessPathExistSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixConfigGetSocketAddressSuccessPathExistTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixConfigGetSocketAddressSuccessPathExist(UNUSED void **state) {
    TEST("elosTestelosUnixConfigGetSocketAddressSuccessPathExist");
    SHOULD("%s", "get the socket address if directories exist");

    elosPlugin_t plugin = {0};
    const char mockPath[] = MOCK_PATH;
    struct sockaddr_un addr = {0};

    MOCK_FUNC_AFTER_CALL(elosUnixConfigGetPath, 0);
    expect_value(elosUnixConfigGetPath, plugin, &plugin);
    will_return(elosUnixConfigGetPath, &mockPath);

    MOCK_FUNC_AFTER_CALL(access, 0);
    expect_value(__wrap_access, pathname, &mockPath);
    expect_any(__wrap_access, mode);
    will_return(__wrap_access, 0);

    MOCK_FUNC_AFTER_CALL(unlink, 0);
    expect_value(__wrap_unlink, pathname, &mockPath);
    will_return(__wrap_unlink, 0);

    safuResultE_t result = elosUnixConfigGetSocketAddress(&plugin, &addr);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(addr.sun_path, MOCK_PATH);
    assert_int_equal(addr.sun_family, AF_UNIX);
}
