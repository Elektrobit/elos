// SPDX-License-Identifier: MIT

#include "mock_samconf.h"
#include "safu/mock_safu.h"
#include "samconfBase64Decode_utest.h"

int samconfTestSamconfBase64DecodeAllocErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfBase64DecodeAllocErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfBase64DecodeAllocError(UNUSED void **state) {
    samconfConfigStatusE_t result;

    const char *base64 =
        "vc7o5/wo8qPZ9aJiSYuJK/s7Ba4kZQA1luddm+YLozpuD1AT2YtkiasFKITbI3xJ"
        "nWmeUFfv9k1mgJ0MEXNK/IF8T89lM Sxk7eXze4zR32FwjKtjCrs7n4EU27+427Q"
        "NE6C1tqmmxpwS07PUBA3ngRCa4n1LhrTciZWOEUMLTqAYsE72zd7yV4z0AMR5b1O"
        "hUuiKMpw2TLBypMRQZ9bQ63tXG4tBHpdCwq7c4C2OQLbbKB0OyrANg4MCSRYcghn"
        "x3Zks1I2NQCGoaK9ir8oYiHHM81UBREvbrT+1ghiu+Xf4O8NNfegSRM79CRsEyf0"
        "p23Qvil7+5RrxuspDBzvgnQ==";
    size_t bufLen = 0;
    uint8_t *buf = (void *)0xdeadb33f;

    const size_t allocSize = (strlen(base64) / 4) * 3;

    TEST("samconfBase64Decode");
    SHOULD("%s", "issue an error on allocation failure");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, allocSize);
    will_return(__wrap_safuAllocMem, NULL);

    result = samconfBase64Decode(base64, &buf, &bufLen);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(buf, (void *)0xdeadb33f);
    assert_int_equal(bufLen, 0);
}
