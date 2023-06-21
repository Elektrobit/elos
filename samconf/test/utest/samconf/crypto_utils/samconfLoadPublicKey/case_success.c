// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "cmocka_mocks/mock_openssl.h"
#include "mock_samconf.h"
#include "safu/mock_safu.h"
#include "samconfLoadPublicKey_utest.h"

int samconfTestSamconfLoadPublicKeySuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfLoadPublicKeySuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfLoadPublicKeySuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;

    const char *keyPath = "/strange/path/to/pub/key.pem";
    FILE *keyFile = (void *)0xdeadf17e;
    EVP_PKEY *pKey = (void *)0xd3add474;
    EVP_PKEY *resKey = NULL;

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, "SAMCONF_SIGNATURE_KEY");
    expect_value(__wrap_safuGetEnvOr, defaultValue, NULL);
    will_return(__wrap_safuGetEnvOr, keyPath);

    MOCK_FUNC_AFTER_CALL(fopen, 0);
    expect_string(__wrap_fopen, filename, keyPath);
    expect_string(__wrap_fopen, mode, "rb");
    will_return(__wrap_fopen, keyFile);

    MOCK_FUNC_AFTER_CALL(PEM_read_PUBKEY, 0);
    expect_value(__wrap_PEM_read_PUBKEY, fp, keyFile);
    expect_value(__wrap_PEM_read_PUBKEY, x, NULL);
    expect_value(__wrap_PEM_read_PUBKEY, cb, NULL);
    expect_value(__wrap_PEM_read_PUBKEY, u, NULL);
    will_return(__wrap_PEM_read_PUBKEY, pKey);

    MOCK_FUNC_AFTER_CALL(fclose, 0);
    expect_value(__wrap_fclose, stream, keyFile);
    will_return(__wrap_fclose, 0);

    result = samconfLoadPublicKey(&resKey);

    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_ptr_equal(resKey, pKey);
}
