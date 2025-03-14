// SPDX-License-Identifier: MIT

#include <errno.h>
#include <mock_unix_config.h>
#include <safu/result.h>
#include <sys/un.h>

#include "elosUnixConfigGetSocketAddress_utest.h"

int elosTestelosUnixConfigGetSocketAddressSuccessCreateDirSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixConfigGetSocketAddressSuccessCreateDirTeardown(UNUSED void **state) {
    MOCK_FUNC_NEVER(mkdir);
    return 0;
}

void elosTestelosUnixConfigGetSocketAddressSuccessCreateDir(UNUSED void **state) {
    TEST("elosTestelosUnixConfigGetSocketAddressSuccessCreateDir");
    SHOULD("%s", "create necessary directories and get the socket address");

    elosPlugin_t plugin = {0};
    const char mockPath[] = MOCK_PATH;
    char mockDirname[] = MOCK_DIR_NAME;
    struct sockaddr_un addr = {0};

    MOCK_FUNC_AFTER_CALL(elosUnixConfigGetPath, 0);
    expect_value(elosUnixConfigGetPath, plugin, &plugin);
    will_return(elosUnixConfigGetPath, &mockPath);

    MOCK_FUNC_AFTER_CALL(access, 0);
    expect_value(__wrap_access, pathname, &mockPath);
    expect_any(__wrap_access, mode);
    will_return(__wrap_access, 1);

    MOCK_FUNC_WRAP(access_errno) = ENOENT;

    MOCK_FUNC_AFTER_CALL(dirname, 0);
    expect_string(__wrap_dirname, path, MOCK_PATH);
    will_return(__wrap_dirname, &mockDirname);

    MOCK_FUNC_ALWAYS(mkdir);
    expect_string(__wrap_mkdir, pathname, DIR);
    expect_any(__wrap_mkdir, mode);
    will_return(__wrap_mkdir, 0);

    expect_string(__wrap_mkdir, pathname, MOCK_DIR_NAME);
    expect_any(__wrap_mkdir, mode);
    will_return(__wrap_mkdir, 0);

    MOCK_FUNC_AFTER_CALL(stat, 0);
    expect_string(__wrap_stat, pathname, MOCK_DIR_NAME);
    expect_any(__wrap_stat, statbuf);
    will_return(__wrap_stat, 0);

    safuResultE_t result = elosUnixConfigGetSocketAddress(&plugin, &addr);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(addr.sun_path, MOCK_PATH);
    assert_int_equal(addr.sun_family, AF_UNIX);
}
