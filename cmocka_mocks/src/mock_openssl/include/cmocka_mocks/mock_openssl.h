// SPDX-License-Identifier: MIT

#ifndef __MOCK_OPENSSL_H__
#define __MOCK_OPENSSL_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_extensions/mock_func_wrap.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

// clang-format off
// because this order is a cmocka requirement
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <cmocka.h>
// clang-format on

MOCK_FUNC_VAR_EXTERN(PEM_read_PUBKEY);
EVP_PKEY *MOCK_FUNC_WRAP(PEM_read_PUBKEY)(FILE *fp, EVP_PKEY **x, pem_password_cb *cb, void *u);
EVP_PKEY *MOCK_FUNC_REAL(PEM_read_PUBKEY)(FILE *fp, EVP_PKEY **x, pem_password_cb *cb, void *u);

MOCK_FUNC_VAR_EXTERN(EVP_DecodeBlock);
int MOCK_FUNC_WRAP(EVP_DecodeBlock)(unsigned char *t, const unsigned char *f, int n);
int MOCK_FUNC_REAL(EVP_DecodeBlock)(unsigned char *t, const unsigned char *f, int n);

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
MOCK_FUNC_VAR_EXTERN(EVP_MD_CTX_new);
EVP_MD_CTX *MOCK_FUNC_WRAP(EVP_MD_CTX_new)(void);
EVP_MD_CTX *MOCK_FUNC_REAL(EVP_MD_CTX_new)(void);
#else
MOCK_FUNC_VAR_EXTERN(EVP_MD_CTX_create);
EVP_MD_CTX *MOCK_FUNC_WRAP(EVP_MD_CTX_create)(void);
EVP_MD_CTX *MOCK_FUNC_REAL(EVP_MD_CTX_create)(void);
#endif

MOCK_FUNC_VAR_EXTERN(EVP_get_digestbyname);
const EVP_MD *MOCK_FUNC_WRAP(EVP_get_digestbyname)(const char *name);
const EVP_MD *MOCK_FUNC_REAL(EVP_get_digestbyname)(const char *name);

MOCK_FUNC_VAR_EXTERN(EVP_DigestVerifyInit);
int MOCK_FUNC_WRAP(EVP_DigestVerifyInit)(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e,
                                         EVP_PKEY *pkey);
int MOCK_FUNC_REAL(EVP_DigestVerifyInit)(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e,
                                         EVP_PKEY *pkey);

MOCK_FUNC_VAR_EXTERN(EVP_DigestUpdate);
int MOCK_FUNC_WRAP(EVP_DigestUpdate)(EVP_MD_CTX *ctx, const void *d, size_t cnt);
int MOCK_FUNC_REAL(EVP_DigestUpdate)(EVP_MD_CTX *ctx, const void *d, size_t cnt);

MOCK_FUNC_VAR_EXTERN(EVP_DigestVerifyFinal);
int MOCK_FUNC_WRAP(EVP_DigestVerifyFinal)(EVP_MD_CTX *ctx, const unsigned char *sig, size_t siglen);
int MOCK_FUNC_REAL(EVP_DigestVerifyFinal)(EVP_MD_CTX *ctx, const unsigned char *sig, size_t siglen);

MOCK_FUNC_VAR_EXTERN(EVP_PKEY_free);
void MOCK_FUNC_WRAP(EVP_PKEY_free)(EVP_PKEY *pkey);
void MOCK_FUNC_REAL(EVP_PKEY_free)(EVP_PKEY *pkey);

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
MOCK_FUNC_VAR_EXTERN(EVP_MD_CTX_free);
void MOCK_FUNC_WRAP(EVP_MD_CTX_free)(EVP_MD_CTX *mdctx);
void MOCK_FUNC_REAL(EVP_MD_CTX_free)(EVP_MD_CTX *mdctx);
#else
MOCK_FUNC_VAR_EXTERN(EVP_MD_CTX_destroy);
void MOCK_FUNC_WRAP(EVP_MD_CTX_destroy)(EVP_MD_CTX *mdctx);
void MOCK_FUNC_REAL(EVP_MD_CTX_destroy)(EVP_MD_CTX *mdctx);
#endif

#endif /* __MOCK_OPENSSL_H__ */
