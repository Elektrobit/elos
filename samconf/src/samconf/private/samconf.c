// SPDX-License-Identifier: MIT

#include "samconf/samconf.h"

#include <safu/common.h>
#include <safu/log.h>
#include <strings.h>

#include "samconf/config_backend.h"
#include "samconf/crypto_utils.h"
#include "samconf/json_backend.h"
#include "samconf/signature.h"

static samconfConfigStatusE_t samconfSupportsDummy(UNUSED const char *location, UNUSED bool *isSupported) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

static samconfConfigStatusE_t samconfOpenDummy(UNUSED const char *location, UNUSED samconfConfigBackend_t *backend) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

static samconfConfigStatusE_t samconfLoadDummy(UNUSED samconfConfigBackend_t *backend, UNUSED bool isSigned,
                                               UNUSED samconfConfig_t **config) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

static samconfConfigStatusE_t samconfCloseDummy(UNUSED samconfConfigBackend_t *backend) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

static const samconfConfigBackendOps_t samconfDummyOps = {
    .supports = samconfSupportsDummy,
    .open = samconfOpenDummy,
    .load = samconfLoadDummy,
    .close = samconfCloseDummy,
};

static const samconfConfigBackendOps_t *const samconfBackendOps[] = {
#ifdef SAMCONF_ENABLE_CONFIG_BACKEND_JSON
    &samconfJsonOps,
#endif
    &samconfDummyOps,
};

NOINLINE const samconfConfigBackendOps_t *samconfGetBackendOps(size_t idx) {
    if (idx < ARRAY_SIZE(samconfBackendOps)) {
        return samconfBackendOps[idx];
    }
    return NULL;
}

NOINLINE samconfConfigStatusE_t samconfLookupBackend(const char *location, samconfConfigBackend_t **backend) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    bool isSupported;

    const samconfConfigBackendOps_t *backendOps = samconfGetBackendOps(0);
    for (size_t i = 0; backendOps != NULL; backendOps = samconfGetBackendOps(++i)) {
        if (backendOps->supports(location, &isSupported) == SAMCONF_CONFIG_OK && isSupported) {
            status = samconfConfigBackendNew(backend, samconfGetBackendOps(i));
            break;
        }
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfVerifySignature(const char *location) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    char *data, *signature;
    size_t signatureLen;
    ssize_t dataLen;

    status = samconfSignatureLoaderGetSignatureFor(location, &signature, &signatureLen);
    if (status == SAMCONF_CONFIG_OK) {
        dataLen = safuReadFileToString(location, -1, &data);
        if (dataLen >= 0) {
            status = samconfCryptoUtilsVerify(data, dataLen, signature);
            if (status != SAMCONF_CONFIG_OK) {
                safuLogErrF("Failed to verify signature of %s.\n", location);
            }

            free(data);
        } else {
            safuLogErrF("Failed to read signature from %s: Read failed.\n", location);
        }

        free(signature);
    } else if (status != SAMCONF_CONFIG_SIGNATURE_NOT_FOUND) {
        safuLogErrF("Failed to load signature: %s.sig.\n", location);
    }

    return status;
}

samconfConfigStatusE_t samconfLoad(const char *location, bool enforceSignature, samconfConfig_t **const config) {
    samconfConfigStatusE_t status, secStatus;
    samconfConfigBackend_t *backend = NULL;
    samconfConfig_t *newConfig = NULL;
    bool isSigned;

    if (config == NULL) {
        safuLogErr("Invalid Call to samconfLoad, unintialized configuration");
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfVerifySignature(location);
    isSigned = (status == SAMCONF_CONFIG_OK);
    if (isSigned || ((status == SAMCONF_CONFIG_SIGNATURE_NOT_FOUND) && !enforceSignature)) {
        status = samconfLookupBackend(location, &backend);
        if (status == SAMCONF_CONFIG_OK) {
            status = backend->ops->open(location, backend);
            if (status == SAMCONF_CONFIG_OK) {
                status = backend->ops->load(backend, isSigned, &newConfig);
                if (status != SAMCONF_CONFIG_OK) {
                    safuLogErrF("Failed to load configuration %s\n", location);
                }

                secStatus = backend->ops->close(backend);
                if (secStatus != SAMCONF_CONFIG_OK) {
                    safuLogErrF("Failed to close configuration %s\n", location);
                    status = secStatus;
                }
            } else {
                safuLogErrF("Failed to open configuration %s.\n", location);
            }

            secStatus = samconfConfigBackendDelete(backend);
            if (secStatus != SAMCONF_CONFIG_OK) {
                safuLogErrF("Failed to delete backend for %s\n", location);
                status = secStatus;
            }

            if (status != SAMCONF_CONFIG_OK && newConfig != NULL) {
                secStatus = samconfConfigDelete(newConfig);
                if (secStatus != SAMCONF_CONFIG_OK) {
                    safuLogErrF("Failed to cleanup after issues while loading %s\n", location);
                }
            }
        } else {
            safuLogErrF("Failed to lookup backend for %s.\n", location);
        }
    } else {
        safuLogErrF("Failed to verify signature for %s.", location);
    }

    if (status == SAMCONF_CONFIG_OK) {
        *config = newConfig;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigNew(samconfConfig_t **const config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        *config = safuAllocMem(NULL, sizeof(samconfConfig_t));
        if (*config != NULL) {
            status = samconfConfigInit(*config);
        }
    }

    return status;
}

samconfConfigStatusE_t samconfConfigInit(samconfConfig_t *config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        memset(config, 0, sizeof(*config));
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigDeleteMembers(samconfConfig_t *config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    if (config) {
        free(config->key);

        if (config->type == SAMCONF_CONFIG_VALUE_STRING) {
            free(config->value.string);
        }

        for (size_t i = 0; i < config->childCount; i++) {
            if (samconfConfigDelete(config->children[i]) == SAMCONF_CONFIG_ERROR) {
                status = SAMCONF_CONFIG_ERROR;
            }
        }

        if (config->childCount > 0) {
            free(config->children);
        }
    } else {
        status = SAMCONF_CONFIG_ERROR;
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfConfigDelete(samconfConfig_t *config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    status = samconfConfigDeleteMembers(config);

    free(config);

    return status;
}

samconfConfigStatusE_t samconfConfigAdd(samconfConfig_t *parent, samconfConfig_t *child) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    if (!parent || !child) {
        status = SAMCONF_CONFIG_ERROR;
        return status;
    }

    parent->children = safuAllocMem(parent->children, sizeof(samconfConfig_t *) * (parent->childCount + 1));

    if (!parent->children) {
        status = SAMCONF_CONFIG_ERROR;
        return status;
    }

    parent->childCount += 1;

    parent->children[parent->childCount - 1] = child;

    child->parent = parent;

    return status;
}

samconfConfigStatusE_t samconfConfigGet(const samconfConfig_t *root, const char *path, const samconfConfig_t **result) {
    size_t childItr = 0;
    size_t childCount = 0;

    if (!root || !root->children || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }
    const samconfConfig_t *node = *(root->children);
    const samconfConfig_t *parent = root;
    childCount = root->childCount;

    char *pathCopy, *freePath, *lastConfig;

    // Two Pointers are needed  at start of string
    // to free them later.
    //
    freePath = pathCopy = strdup(path);

    if (!pathCopy || !freePath) {
        return SAMCONF_CONFIG_ERROR;
    }

    if (pathCopy[0] == '\0') {
        free(freePath);
        return SAMCONF_CONFIG_NOT_FOUND;
    }

    if (pathCopy[0] == '/') {
        if (strlen(pathCopy) == 1) {
            free(freePath);
            return SAMCONF_CONFIG_NOT_FOUND;
        } else {
            ++pathCopy;
        }
    }

    if (pathCopy[strlen(pathCopy) - 1] == '/') {
        pathCopy[strlen(pathCopy) - 1] = '\0';
    }

    lastConfig = strrchr(pathCopy, '/');
    if (lastConfig) {
        ++lastConfig;
    } else {
        lastConfig = pathCopy;
    }

    char *token = strsep(&pathCopy, "/");

    while (token) {
        for (childItr = 0; childItr < childCount; childItr++) {
            node = parent->children[childItr];
            if (strcmp(node->key, token) == 0) {
                if (token == lastConfig) {
                    *result = node;
                    free(freePath);
                    return SAMCONF_CONFIG_OK;
                } else if (node->children) {
                    childItr = 0;
                    childCount = node->childCount;
                    parent = node;
                    break;
                } else {
                    free(freePath);
                    return SAMCONF_CONFIG_NOT_FOUND;
                }
            }
        }

        if ((childCount != 0) && (childItr == childCount)) {
            free(freePath);
            return SAMCONF_CONFIG_NOT_FOUND;
        }

        token = strsep(&pathCopy, "/");
    }

    free(freePath);
    return SAMCONF_CONFIG_NOT_FOUND;
}

samconfConfigStatusE_t samconfConfigGetString(const samconfConfig_t *root, const char *path, const char **result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (!node || (SAMCONF_CONFIG_OK != status)) {
        *result = NULL;
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_STRING) {
        *result = node->value.string;
    } else {
        *result = NULL;
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetString(samconfConfig_t *config, const char *stringValue) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL && stringValue != NULL) {
        config->value.string = strdup(stringValue);
        if (config->value.string != NULL) {
            config->type = SAMCONF_CONFIG_VALUE_STRING;
            status = SAMCONF_CONFIG_OK;
        }
    }

    return status;
}

samconfConfigStatusE_t samconfConfigGetBool(const samconfConfig_t *root, const char *path, bool *result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (SAMCONF_CONFIG_OK != status) {
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_BOOLEAN) {
        *result = node->value.boolean;
    } else {
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigGetInt32(const samconfConfig_t *root, const char *path, int32_t *result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (SAMCONF_CONFIG_OK != status) {
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_INT) {
        *result = node->value.integer;
    } else {
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetInt(samconfConfig_t *config, int64_t intValue) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        config->value.integer = intValue;
        config->type = SAMCONF_CONFIG_VALUE_INT;
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetBool(samconfConfig_t *config, bool value) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        config->value.boolean = value;
        config->type = SAMCONF_CONFIG_VALUE_BOOLEAN;
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetReal(samconfConfig_t *config, double value) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        config->value.real = value;
        config->type = SAMCONF_CONFIG_VALUE_REAL;
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigGetReal(const samconfConfig_t *root, const char *path, double *result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (SAMCONF_CONFIG_OK != status) {
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_REAL) {
        *result = node->value.real;
    } else {
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

int samconfInitConfig() {
    return 0;
}
