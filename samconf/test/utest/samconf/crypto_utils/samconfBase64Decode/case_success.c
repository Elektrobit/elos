// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_openssl.h"
#include "mock_samconf.h"
#include "safu/mock_safu.h"
#include "samconfBase64Decode_utest.h"

int samconfTestSamconfBase64DecodeSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfBase64DecodeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfBase64DecodeSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;

    const char *base64 =
        "vc7o5/wo8qPZ9aJiSYuJK/s7Ba4kZQA1luddm+YLozpuD1AT2YtkiasFKITbI3xJ"
        "nWmeUFfv9k1mgJ0MEXNK/IF8T89lM Sxk7eXze4zR32FwjKtjCrs7n4EU27+427Q"
        "NE6C1tqmmxpwS07PUBA3ngRCa4n1LhrTciZWOEUMLTqAYsE72zd7yV4z0AMR5b1O"
        "hUuiKMpw2TLBypMRQZ9bQ63tXG4tBHpdCwq7c4C2OQLbbKB0OyrANg4MCSRYcghn"
        "x3Zks1I2NQCGoaK9ir8oYiHHM81UBREvbrT+1ghiu+Xf4O8NNfegSRM79CRsEyf0"
        "p23Qvil7+5RrxuspDBzvgnQ==";
    size_t bufLen = 0;
    uint8_t *buf;

    uint8_t *allocMem = (void *)0xdeadda7a;
    const size_t allocSize = (strlen(base64) / 4) * 3;

    TEST("samconfBase64Decode");
    SHOULD("%s", "return the raw stream for the given base64 data");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, allocSize);
    will_return(__wrap_safuAllocMem, allocMem);

    MOCK_FUNC_AFTER_CALL(EVP_DecodeBlock, 0);
    expect_value(__wrap_EVP_DecodeBlock, t, allocMem);
    expect_value(__wrap_EVP_DecodeBlock, f, (const unsigned char *)base64);
    expect_value(__wrap_EVP_DecodeBlock, n, strlen(base64));
    will_return(__wrap_EVP_DecodeBlock, allocSize);

    result = samconfBase64Decode(base64, &buf, &bufLen);

    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_ptr_equal(buf, allocMem);
    /* Do not forget to remove the two padding characters */
    assert_int_equal(bufLen, allocSize - 2);
}
