// SPDX-License-Identifier: MIT

#include "mock_samconf.h"

MOCK_FUNC_BODY(samconfGetBackendOps, const samconfConfigBackendOps_t *, size_t idx) {
    if (MOCK_IS_ACTIVE(samconfGetBackendOps)) {
        check_expected(idx);
        return mock_type(const samconfConfigBackendOps_t *);
    }
    return MOCK_FUNC_REAL(samconfGetBackendOps)(idx);
}

MOCK_FUNC_BODY(samconfLookupBackend, samconfConfigStatusE_t, const char *location, samconfConfigBackend_t **backend) {
    if (MOCK_IS_ACTIVE(samconfLookupBackend)) {
        check_expected_ptr(location);
        check_expected_ptr(backend);
        if (backend != NULL) {
            *backend = mock_ptr_type(samconfConfigBackend_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfLookupBackend)(location, backend);
}

MOCK_FUNC_BODY(samconfVerifySignature, samconfConfigStatusE_t, const char *location) {
    if (MOCK_IS_ACTIVE(samconfVerifySignature)) {
        check_expected_ptr(location);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfVerifySignature)(location);
}

MOCK_FUNC_BODY(samconfConfigBackendNew, samconfConfigStatusE_t, samconfConfigBackend_t **backend,
               const samconfConfigBackendOps_t *ops) {
    if (MOCK_IS_ACTIVE(samconfConfigBackendNew)) {
        check_expected_ptr(backend);
        check_expected_ptr(ops);
        if (backend != NULL) {
            *backend = mock_ptr_type(samconfConfigBackend_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigBackendNew)(backend, ops);
}

MOCK_FUNC_BODY(samconfConfigBackendInit, samconfConfigStatusE_t, samconfConfigBackend_t *backend,
               const samconfConfigBackendOps_t *ops) {
    if (MOCK_IS_ACTIVE(samconfConfigBackendInit)) {
        check_expected_ptr(backend);
        check_expected_ptr(ops);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigBackendInit)(backend, ops);
}

MOCK_FUNC_BODY(samconfConfigBackendDelete, samconfConfigStatusE_t, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfConfigBackendDelete)) {
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigBackendDelete)(backend);
}

MOCK_FUNC_BODY(samconfConfigGet, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               const samconfConfig_t **result) {
    if (MOCK_IS_ACTIVE(samconfConfigGet)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_ptr_type(const samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGet)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigGetBool, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               bool *result) {
    if (MOCK_IS_ACTIVE(samconfConfigGetBool)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_type(bool);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetBool)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigGetInt32, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               int32_t *result) {
    if (MOCK_IS_ACTIVE(samconfConfigGetInt32)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_type(int32_t);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetInt32)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigGetString, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               const char **result) {
    if (MOCK_IS_ACTIVE(samconfConfigGetString)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_ptr_type(const char *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetString)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigDelete, samconfConfigStatusE_t, samconfConfig_t *config) {
    if (MOCK_IS_ACTIVE(samconfConfigDelete)) {
        check_expected_ptr(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigDelete)(config);
}

MOCK_FUNC_BODY(samconfConfigDeleteMembers, samconfConfigStatusE_t, samconfConfig_t *config) {
    if (MOCK_IS_ACTIVE(samconfConfigDeleteMembers)) {
        check_expected_ptr(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigDeleteMembers)(config);
}

MOCK_FUNC_BODY(samconfConfigNew, samconfConfigStatusE_t, samconfConfig_t **config) {
    if (MOCK_IS_ACTIVE(samconfConfigNew)) {
        check_expected(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigNew)(config);
}

MOCK_FUNC_BODY(samconfConfigAdd, samconfConfigStatusE_t, samconfConfig_t *parent, samconfConfig_t *child) {
    if (MOCK_IS_ACTIVE(samconfConfigAdd)) {
        check_expected(parent);
        check_expected(child);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigAdd)(parent, child);
}

MOCK_FUNC_BODY(samconfJsonBackendSupports, samconfConfigStatusE_t, const char *location, bool *isSupported) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendSupports)) {
        check_expected_ptr(location);
        check_expected_ptr(isSupported);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendSupports)(location, isSupported);
}

MOCK_FUNC_BODY(samconfJsonBackendOpen, samconfConfigStatusE_t, const char *location, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendOpen)) {
        check_expected_ptr(location);
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendOpen)(location, backend);
}

MOCK_FUNC_BODY(samconfJsonBackendLoad, samconfConfigStatusE_t, samconfConfigBackend_t *backend, bool isSigned,
               samconfConfig_t **config) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendLoad)) {
        check_expected_ptr(backend);
        check_expected(isSigned);
        check_expected_ptr(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendLoad)(backend, isSigned, config);
}

MOCK_FUNC_BODY(samconfJsonBackendClose, samconfConfigStatusE_t, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendClose)) {
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendClose)(backend);
}

MOCK_FUNC_BODY(samconfUriNew, samconfConfigStatusE_t, samconfUri_t **uri, const char *uriString) {
    if (MOCK_IS_ACTIVE(samconfUriNew)) {
        check_expected_ptr(uri);
        check_expected(uriString);
        if (uri != NULL) {
            *uri = mock_ptr_type(samconfUri_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriNew)(uri, uriString);
}

MOCK_FUNC_BODY(samconfUriInit, samconfConfigStatusE_t, samconfUri_t *uri, const char *uriString) {
    if (MOCK_IS_ACTIVE(samconfUriInit)) {
        check_expected_ptr(uri);
        check_expected(uriString);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriInit)(uri, uriString);
}

MOCK_FUNC_BODY(samconfUriDelete, samconfConfigStatusE_t, samconfUri_t *uri) {
    if (MOCK_IS_ACTIVE(samconfUriDelete)) {
        check_expected_ptr(uri);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriDelete)(uri);
}

MOCK_FUNC_BODY(samconfUriDeleteMembers, samconfConfigStatusE_t, samconfUri_t *uri) {
    if (MOCK_IS_ACTIVE(samconfUriDeleteMembers)) {
        check_expected_ptr(uri);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriDeleteMembers)(uri);
}

MOCK_FUNC_BODY(samconfUriPattern, samconfConfigStatusE_t, regex_t **pattern) {
    if (MOCK_IS_ACTIVE(samconfUriPattern)) {
        check_expected_ptr(pattern);
        if (pattern != NULL) {
            *pattern = mock_ptr_type(regex_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriPattern)(pattern);
}

MOCK_FUNC_BODY(samconfLoadPublicKey, samconfConfigStatusE_t, EVP_PKEY **pKey) {
    if (MOCK_IS_ACTIVE(samconfLoadPublicKey)) {
        check_expected_ptr(pKey);
        if (pKey != NULL) {
            *pKey = mock_ptr_type(EVP_PKEY *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfLoadPublicKey)(pKey);
}

MOCK_FUNC_BODY(samconfBase64Decode, samconfConfigStatusE_t, const char *base64, uint8_t **buf, size_t *bufLen) {
    if (MOCK_IS_ACTIVE(samconfBase64Decode)) {
        check_expected(base64);
        check_expected_ptr(buf);
        check_expected_ptr(bufLen);
        if (buf != NULL) {
            *buf = mock_ptr_type(uint8_t *);
        }
        if (bufLen != NULL) {
            *bufLen = mock_type(size_t);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfBase64Decode)(base64, buf, bufLen);
}

MOCK_FUNC_BODY(samconfCryptoUtilsVerify, samconfConfigStatusE_t, const char *data, size_t length,
               const char *signature) {
    if (MOCK_IS_ACTIVE(samconfCryptoUtilsVerify)) {
        check_expected_ptr(data);
        check_expected(length);
        check_expected_ptr(signature);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfCryptoUtilsVerify)(data, length, signature);
}
