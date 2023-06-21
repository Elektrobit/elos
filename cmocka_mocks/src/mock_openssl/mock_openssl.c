// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_openssl.h"

#include <cmocka.h>
#include <string.h>

MOCK_FUNC_VAR_NEW(PEM_read_PUBKEY);
EVP_PKEY *MOCK_FUNC_WRAP(PEM_read_PUBKEY)(FILE *fp, EVP_PKEY **x, pem_password_cb *cb, void *u) {
    if (MOCK_IS_ACTIVE(PEM_read_PUBKEY)) {
        check_expected_ptr(fp);
        check_expected_ptr(x);
        check_expected_ptr(cb);
        check_expected_ptr(u);
        return mock_ptr_type(EVP_PKEY *);
    }
    return MOCK_FUNC_REAL(PEM_read_PUBKEY)(fp, x, cb, u);
}

MOCK_FUNC_VAR_NEW(EVP_DecodeBlock);
int MOCK_FUNC_WRAP(EVP_DecodeBlock)(unsigned char *t, const unsigned char *f, int n) {
    if (MOCK_IS_ACTIVE(EVP_DecodeBlock)) {
        check_expected_ptr(t);
        check_expected(f);
        check_expected(n);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(EVP_DecodeBlock)(t, f, n);
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
MOCK_FUNC_VAR_NEW(EVP_MD_CTX_new);
EVP_MD_CTX *MOCK_FUNC_WRAP(EVP_MD_CTX_new)(void) {
    if (MOCK_IS_ACTIVE(EVP_MD_CTX_new)) {
        return mock_ptr_type(EVP_MD_CTX *);
    }

    return MOCK_FUNC_REAL(EVP_MD_CTX_new)();
}
#else
MOCK_FUNC_VAR_NEW(EVP_MD_CTX_create);
EVP_MD_CTX *MOCK_FUNC_WRAP(EVP_MD_CTX_create)(void) {
    if (MOCK_IS_ACTIVE(EVP_MD_CTX_create)) {
        return mock_ptr_type(EVP_MD_CTX *);
    }

    return MOCK_FUNC_REAL(EVP_MD_CTX_create)();
}
#endif

MOCK_FUNC_VAR_NEW(EVP_get_digestbyname);
const EVP_MD *MOCK_FUNC_WRAP(EVP_get_digestbyname)(const char *name) {
    if (MOCK_IS_ACTIVE(EVP_get_digestbyname)) {
        check_expected(name);
        return mock_ptr_type(const EVP_MD *);
    }
    return MOCK_FUNC_REAL(EVP_get_digestbyname)(name);
}

MOCK_FUNC_VAR_NEW(EVP_DigestVerifyInit);
int MOCK_FUNC_WRAP(EVP_DigestVerifyInit)(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e,
                                         EVP_PKEY *pkey) {
    if (MOCK_IS_ACTIVE(EVP_DigestVerifyInit)) {
        check_expected_ptr(ctx);
        check_expected_ptr(pctx);
        check_expected_ptr(type);
        check_expected_ptr(e);
        check_expected_ptr(pkey);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(EVP_DigestVerifyInit)(ctx, pctx, type, e, pkey);
}

MOCK_FUNC_VAR_NEW(EVP_DigestUpdate);
int MOCK_FUNC_WRAP(EVP_DigestUpdate)(EVP_MD_CTX *ctx, const void *d, size_t cnt) {
    if (MOCK_IS_ACTIVE(EVP_DigestUpdate)) {
        check_expected_ptr(ctx);
        check_expected_ptr(d);
        check_expected(cnt);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(EVP_DigestUpdate)(ctx, d, cnt);
}

MOCK_FUNC_VAR_NEW(EVP_DigestVerifyFinal);
int MOCK_FUNC_WRAP(EVP_DigestVerifyFinal)(EVP_MD_CTX *ctx, const unsigned char *sig, size_t siglen) {
    if (MOCK_IS_ACTIVE(EVP_DigestVerifyFinal)) {
        check_expected_ptr(ctx);
        check_expected_ptr(sig);
        check_expected(siglen);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(EVP_DigestVerifyFinal)(ctx, sig, siglen);
}

MOCK_FUNC_VAR_NEW(EVP_PKEY_free);
void MOCK_FUNC_WRAP(EVP_PKEY_free)(EVP_PKEY *pkey) {
    if (MOCK_IS_ACTIVE(EVP_PKEY_free)) {
        check_expected_ptr(pkey);
        return;
    }
    MOCK_FUNC_REAL(EVP_PKEY_free)(pkey);
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
MOCK_FUNC_VAR_NEW(EVP_MD_CTX_free);
void MOCK_FUNC_WRAP(EVP_MD_CTX_free)(EVP_MD_CTX *mdctx) {
    if (MOCK_IS_ACTIVE(EVP_MD_CTX_free)) {
        check_expected_ptr(mdctx);
        return;
    }
    MOCK_FUNC_REAL(EVP_MD_CTX_free)(mdctx);
}
#else
MOCK_FUNC_VAR_NEW(EVP_MD_CTX_destroy);
void MOCK_FUNC_WRAP(EVP_MD_CTX_destroy)(EVP_MD_CTX *mdctx) {
    if (MOCK_IS_ACTIVE(EVP_MD_CTX_destroy)) {
        check_expected_ptr(mdctx);
        return;
    }
    MOCK_FUNC_REAL(EVP_MD_CTX_destroy)(mdctx);
}
#endif
