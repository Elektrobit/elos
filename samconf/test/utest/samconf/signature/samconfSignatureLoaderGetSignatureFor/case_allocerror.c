// SPDX-License-Identifier: MIT
#include <samconf/signature.h>

#include "cmocka_mocks/mock_libc.h"
#include "safu/mock_safu.h"
#include "samconf/samconf.h"
#include "samconfSignatureLoaderGetSignatureFor_utest.h"

int samconfTestSamconfSignatureLoaderGetSignatureForAllocErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfSignatureLoaderGetSignatureForAllocErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfSignatureLoaderGetSignatureForAllocError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    char *signature = NULL;
    size_t len = -0xc0f3d00d;

    const char *path = "/my/path/to/config.json";
    const size_t pathLen = strlen(path);
    const size_t sigPathLen = pathLen + 4;

    TEST("samconfSignatureLoaderGetSignatureFor");
    SHOULD("%s", "return error when failing to read signature file");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(char) * (sigPathLen + 1));
    will_return(__wrap_safuAllocMem, NULL);

    status = samconfSignatureLoaderGetSignatureFor(path, &signature, &len);

    assert_ptr_equal(len, -0xc0f3d00d);
    assert_ptr_equal(signature, NULL);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
