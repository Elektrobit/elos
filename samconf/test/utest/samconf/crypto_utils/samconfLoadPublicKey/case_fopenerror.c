// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safu/mock_safu.h"
#include "samconfLoadPublicKey_utest.h"

int samconfTestSamconfLoadPublicKeyFopenErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfLoadPublicKeyFopenErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfLoadPublicKeyFopenError(UNUSED void **state) {
    samconfConfigStatusE_t result;

    const char *keyPath = "/strange/path/to/pub/key.pem";
    EVP_PKEY *resKey = (void *)0xdeadda7a;

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, "SAMCONF_SIGNATURE_KEY");
    expect_value(__wrap_safuGetEnvOr, defaultValue, NULL);
    will_return(__wrap_safuGetEnvOr, keyPath);

    MOCK_FUNC_AFTER_CALL(fopen, 0);
    expect_string(__wrap_fopen, filename, keyPath);
    expect_string(__wrap_fopen, mode, "rb");
    will_return(__wrap_fopen, NULL);

    result = samconfLoadPublicKey(&resKey);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(resKey, (void *)0xdeadda7a);
}
