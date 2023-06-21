// SPDX-License-Identifier: MIT
#include "samconf/crypto_utils.h"

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <regex.h>
#include <safu/common.h>
#include <safu/log.h>
#include <string.h>

#include "samconf/uri.h"

NOINLINE samconfConfigStatusE_t samconfLoadPublicKey(EVP_PKEY **pKey) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    const char *pKeyPath = safuGetEnvOr("SAMCONF_SIGNATURE_KEY", NULL);
    EVP_PKEY *tmp;
    FILE *sigFd;

    if (pKeyPath != NULL) {
        sigFd = fopen(pKeyPath, "rb");
        if (sigFd != NULL) {
            tmp = PEM_read_PUBKEY(sigFd, NULL, NULL, NULL);
            if (tmp != NULL) {
                status = SAMCONF_CONFIG_OK;
                *pKey = tmp;
            } else {
                safuLogErrF("Failed to read public key from %s.\n", pKeyPath);
            }

            fclose(sigFd);
        } else {
            safuLogErrF("Failed to open public key file %s.\n", pKeyPath);
        }
    } else {
        safuLogErr("Failed to fetch public key path from 'SAMCONF_SIGNATURE_KEY'.\n");
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfBase64Decode(const char *base64, uint8_t **buf, size_t *bufLen) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    uint8_t *tmp;
    size_t padding = 0;
    int inLen = strlen(base64);
    int outLen = (inLen / 4) * 3;

    tmp = safuAllocMem(NULL, outLen);
    if (tmp != NULL) {
        outLen = EVP_DecodeBlock(tmp, (const unsigned char *)base64, strlen(base64));
        if (outLen >= 0) {
            while (base64[--inLen] == '=') {
                --outLen;
                if (++padding > 2) {
                    safuLogErrF("Failed to decode base64 string because of invalid passing: %s.\n", base64);
                    break;
                }
            }

            if (padding <= 2) {
                *buf = tmp;
                *bufLen = (size_t)outLen;
                status = SAMCONF_CONFIG_OK;
            }
        } else {
            safuLogErrF("Failed to decode base64 string: %s.\n", base64);
            free(tmp);
        }
    } else {
        safuLogErr("Failed to allocate base64 output buffer.\n");
    }

    return status;
}

samconfConfigStatusE_t samconfCryptoUtilsVerify(const char *data, size_t length, const char *signature) {
    samconfConfigStatusE_t secStatus, status = SAMCONF_CONFIG_ERROR;

    uint8_t *rawSig = NULL;
    size_t rawSigLen = 0;

    EVP_MD_CTX *mdCtx = NULL;
    const EVP_MD *sigAlg = NULL;
    EVP_PKEY *pKey = NULL;
    samconfUri_t *uri;

    if (data != NULL && signature != NULL) {
        secStatus = samconfUriNew(&uri, signature);
        if (secStatus == SAMCONF_CONFIG_OK) {
            secStatus = samconfLoadPublicKey(&pKey);
            if (secStatus == SAMCONF_CONFIG_OK) {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
                mdCtx = EVP_MD_CTX_new();
#else
                mdCtx = EVP_MD_CTX_create();
#endif
                if (mdCtx != NULL) {
                    secStatus = samconfBase64Decode(&uri->path[1], &rawSig, &rawSigLen);
                    if (secStatus == SAMCONF_CONFIG_OK) {
                        const char *digestName = uri->scheme;
                        if (strcmp(digestName, "RSA-SHA2-256") == 0) {
                            digestName = "SHA256";
                        }
                        sigAlg = EVP_get_digestbyname(digestName);
                        if (sigAlg != NULL) {
                            if (EVP_DigestVerifyInit(mdCtx, NULL, sigAlg, NULL, pKey) == 1) {
                                if (EVP_DigestUpdate(mdCtx, data, length) == 1) {
                                    if (EVP_DigestVerifyFinal(mdCtx, rawSig, rawSigLen) == 1) {
                                        status = SAMCONF_CONFIG_OK;
                                    } else {
                                        safuLogErrF("Failed to verify signature: %s.\n", signature);
                                        status = SAMCONF_CONFIG_INVALID_SIGNATURE;
                                    }
                                } else {
                                    safuLogErrF("Failed to update signature verification for %s.\n", signature);
                                }
                            } else {
                                safuLogErrF("Failed to init signature verification for %s.\n", signature);
                            }
                        } else {
                            safuLogErrF("Failed to fetch digest algorithm for %s.\n", uri->scheme);
                        }
                    } else {
                        safuLogErrF("Failed to decode base64 encoded signature: %s.\n", uri->path);
                    }
                } else {
                    safuLogErrF("Failed to init OpenSSL digest context for %s.\n", signature);
                }
            } else {
                safuLogErrF("Failed to load public key to check signature %s.\n", signature);
            }

            EVP_PKEY_free(pKey);
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
            EVP_MD_CTX_free(mdCtx);
#else
            EVP_MD_CTX_destroy(mdCtx);
#endif

            free(rawSig);

            secStatus = samconfUriDelete(uri);
            if (secStatus != SAMCONF_CONFIG_OK) {
                safuLogErrF("Failed to delete uri object for %s.\n", signature);
                status = SAMCONF_CONFIG_ERROR;
            }
        }
    }

    return status;
}
