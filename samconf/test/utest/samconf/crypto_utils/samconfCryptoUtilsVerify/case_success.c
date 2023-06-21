// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "cmocka_mocks/mock_openssl.h"
#include "mock_samconf.h"
#include "samconfCryptoUtilsVerify_utest.h"

int samconfTestSamconfCryptoUtilsVerifySuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCryptoUtilsVerifySuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCryptoUtilsVerifySuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;

    const char *data = (void *)0xd3add474;
    const char *signature =
        "RSA-SHA2-256:///vc7o5/wo8qPZ9aJiSYuJK/s7Ba4kZQA1luddm+YLozpuD1AT"
        "2YtkiasFKITbI3xJnWmeUFfv9k1mgJ0MEXNK/IF8T89lM Sxk7eXze4zR32FwjKt"
        "jCrs7n4EU27+427QNE6C1tqmmxpwS07PUBA3ngRCa4n1LhrTciZWOEUMLTqAYsE7"
        "2zd7yV4z0AMR5b1OhUuiKMpw2TLBypMRQZ9bQ63tXG4tBHpdCwq7c4C2OQLbbKB0"
        "OyrANg4MCSRYcghnx3Zks1I2NQCGoaK9ir8oYiHHM81UBREvbrT+1ghiu+Xf4O8N"
        "NfegSRM79CRsEyf0p23Qvil7+5RrxuspDBzvgnQ==";
    size_t dataLen = 0x4000;

    uint8_t *rawSig = (void *)0xb33fc0f3;
    size_t rawSigLen = 0x100;

    EVP_MD_CTX *evpCtx = (void *)0xd3adb33f;
    EVP_MD *md = (void *)0xdeadd474;
    EVP_PKEY *pKey = (void *)0xc0fe3eee;

    samconfUri_t uri = {
        .scheme = "RSA-SHA2-256",
        .path =
            "/vc7o5/wo8qPZ9aJiSYuJK/s7Ba4kZQA1luddm+YLozpuD1AT2YtkiasFKITbI3x"
            "JnWmeUFfv9k1mgJ0MEXNK/IF8T89lM Sxk7eXze4zR32FwjKtjCrs7n4EU27+427"
            "QNE6C1tqmmxpwS07PUBA3ngRCa4n1LhrTciZWOEUMLTqAYsE72zd7yV4z0AMR5b1"
            "OhUuiKMpw2TLBypMRQZ9bQ63tXG4tBHpdCwq7c4C2OQLbbKB0OyrANg4MCSRYcgh"
            "nx3Zks1I2NQCGoaK9ir8oYiHHM81UBREvbrT+1ghiu+Xf4O8NNfegSRM79CRsEyf"
            "0p23Qvil7+5RrxuspDBzvgnQ==",
    };

    TEST("samconfCryptoUtilsVerify");
    SHOULD("%s", "return OK when given a matching sha1 for a data string");

    MOCK_FUNC_AFTER_CALL(samconfUriNew, 0);
    expect_any(samconfUriNew, uri);
    expect_value(samconfUriNew, uriString, signature);
    will_set_parameter(samconfUriNew, uri, &uri);
    will_return(samconfUriNew, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfLoadPublicKey, 0);
    expect_any(samconfLoadPublicKey, pKey);
    will_set_parameter(samconfLoadPublicKey, pKey, pKey);
    will_return(samconfLoadPublicKey, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(EVP_MD_CTX_new, 0);
    will_return(__wrap_EVP_MD_CTX_new, evpCtx);

    MOCK_FUNC_AFTER_CALL(samconfBase64Decode, 0);
    expect_string(samconfBase64Decode, base64, &uri.path[1]);
    expect_any(samconfBase64Decode, buf);
    expect_any(samconfBase64Decode, bufLen);
    will_set_parameter(samconfBase64Decode, buf, rawSig);
    will_set_parameter(samconfBase64Decode, bufLen, rawSigLen);
    will_return(samconfBase64Decode, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(EVP_get_digestbyname, 0);
    expect_string(__wrap_EVP_get_digestbyname, name, "SHA256");
    will_return(__wrap_EVP_get_digestbyname, md);

    MOCK_FUNC_AFTER_CALL(EVP_DigestVerifyInit, 0);
    expect_value(__wrap_EVP_DigestVerifyInit, ctx, evpCtx);
    expect_value(__wrap_EVP_DigestVerifyInit, pctx, NULL);
    expect_value(__wrap_EVP_DigestVerifyInit, type, md);
    expect_value(__wrap_EVP_DigestVerifyInit, e, NULL);
    expect_value(__wrap_EVP_DigestVerifyInit, pkey, pKey);
    will_return(__wrap_EVP_DigestVerifyInit, 1);

    MOCK_FUNC_AFTER_CALL(EVP_DigestUpdate, 0);
    expect_value(__wrap_EVP_DigestUpdate, ctx, evpCtx);
    expect_value(__wrap_EVP_DigestUpdate, d, data);
    expect_value(__wrap_EVP_DigestUpdate, cnt, dataLen);
    will_return(__wrap_EVP_DigestUpdate, 1);

    MOCK_FUNC_AFTER_CALL(EVP_DigestVerifyFinal, 0);
    expect_value(__wrap_EVP_DigestVerifyFinal, ctx, evpCtx);
    expect_value(__wrap_EVP_DigestVerifyFinal, sig, rawSig);
    expect_value(__wrap_EVP_DigestVerifyFinal, siglen, rawSigLen);
    will_return(__wrap_EVP_DigestVerifyFinal, 1);

    MOCK_FUNC_AFTER_CALL(EVP_PKEY_free, 0);
    expect_value(__wrap_EVP_PKEY_free, pkey, pKey);

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    MOCK_FUNC_AFTER_CALL(EVP_MD_CTX_free, 0);
    expect_value(__wrap_EVP_MD_CTX_free, mdctx, evpCtx);
#else
    MOCK_FUNC_AFTER_CALL(EVP_MD_CTX_destroy, 0);
    expect_value(__wrap_EVP_MD_CTX_destroy, mdctx, evpCtx);
#endif

    MOCK_FUNC_AFTER_CALL(free, 0);
    expect_value(__wrap_free, ptr, rawSig);

    MOCK_FUNC_AFTER_CALL(samconfUriDelete, 0);
    expect_value(samconfUriDelete, uri, &uri);
    will_return(samconfUriDelete, SAMCONF_CONFIG_OK);

    result = samconfCryptoUtilsVerify(data, dataLen, signature);

    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
