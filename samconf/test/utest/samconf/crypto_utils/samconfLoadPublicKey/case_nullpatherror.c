// SPDX-License-Identifier: MIT

#include "safu/mock_safu.h"
#include "samconfLoadPublicKey_utest.h"

int samconfTestSamconfLoadPublicKeyNullPathErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfLoadPublicKeyNullPathErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfLoadPublicKeyNullPathError(UNUSED void **state) {
    samconfConfigStatusE_t result;

    EVP_PKEY *resKey = (void *)0xdeadda7a;

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, "SAMCONF_SIGNATURE_KEY");
    expect_value(__wrap_safuGetEnvOr, defaultValue, NULL);
    will_return(__wrap_safuGetEnvOr, NULL);

    result = samconfLoadPublicKey(&resKey);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(resKey, (void *)0xdeadda7a);
}
