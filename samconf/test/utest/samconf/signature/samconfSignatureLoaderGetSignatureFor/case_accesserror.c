// SPDX-License-Identifier: MIT
#include <errno.h>
#include <samconf/signature.h>

#include "cmocka_mocks/mock_libc.h"
#include "safu/mock_safu.h"
#include "samconf/samconf.h"
#include "samconfSignatureLoaderGetSignatureFor_utest.h"

int samconfTestSamconfSignatureLoaderGetSignatureForAccessErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfSignatureLoaderGetSignatureForAccessErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfSignatureLoaderGetSignatureForAccessError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    char *signature = NULL;
    size_t len = -0xc0f3d00d;

    const char *path = "/my/path/to/config.json";
    const size_t pathLen = strlen(path);
    const size_t sigPathLen = pathLen + 4;
    char sigPath[0x100] = {1};

    TEST("samconfSignatureLoaderGetSignatureFor");
    SHOULD("%s", "return error when failing to read signature file");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(char) * (sigPathLen + 1));
    will_return(__wrap_safuAllocMem, sigPath);

    MOCK_FUNC_AFTER_CALL(access, 0);
    expect_value(__wrap_access, pathname, sigPath);
    expect_value(__wrap_access, mode, R_OK | F_OK);
    will_return(__wrap_access, ENOENT);

    MOCK_FUNC_AFTER_CALL(free, 0);
    expect_value(__wrap_free, ptr, sigPath);

    status = samconfSignatureLoaderGetSignatureFor(path, &signature, &len);

    assert_ptr_equal(len, -0xc0f3d00d);
    assert_ptr_equal(signature, NULL);
    assert_int_equal(status, SAMCONF_CONFIG_SIGNATURE_NOT_FOUND);
}
