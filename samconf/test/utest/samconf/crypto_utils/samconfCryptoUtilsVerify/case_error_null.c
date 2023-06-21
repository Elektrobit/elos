// SPDX-License-Identifier: MIT

#include "samconfCryptoUtilsVerify_utest.h"

int samconfTestSamconfCryptoUtilsVerifyErrorNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCryptoUtilsVerifyErrorNullTeardown(UNUSED void **state) {
    return 0;
}

static void _testSamconfCryptoUtilsVerifyErrorNullParam(const char *data, size_t lenght, const char *signature) {
    samconfConfigStatusE_t result;

    result = samconfCryptoUtilsVerify(data, lenght, signature);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}

void samconfTestSamconfCryptoUtilsVerifyErrorNull(UNUSED void **state) {
    TEST("samconfCryptoUtilsVerify");
    SHOULD("%s", "return Error when data or signature is null");

    _testSamconfCryptoUtilsVerifyErrorNullParam("data", 4, NULL);
    _testSamconfCryptoUtilsVerifyErrorNullParam(NULL, 0, "signature");
}
