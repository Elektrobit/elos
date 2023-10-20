// SPDX-License-Identifier: MIT

#include "shmem_config.h"

#include <safu/common.h>
#include <safu/log.h>
#include <samconf/samconf.h>

#include "elos/config/defines.h"
#include "shmem.h"

static safuResultE_t _getString(samconfConfig_t const *config, char const *from, char **to) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfigStatusE_t configResult;
    char const *value = NULL;

    configResult = samconfConfigGetString(config, from, &value);
    if (configResult == SAMCONF_CONFIG_NOT_FOUND) {
        safuLogErrF("Field is missing in the configuration: '%s'", from);
    } else if (configResult != SAMCONF_CONFIG_OK) {
        safuLogErrValue("samconfConfigGetString failed.", configResult);
    } else {
        result = safuStringIsEmpty(value);
        if (result == SAFU_RESULT_OK) {
            safuLogErrF("Field has invalid value (empty string) in the configuration: '%s'", from);
            result = SAFU_RESULT_FAILED;
        } else {
            char *newString = NULL;

            newString = strdup(value);
            if (newString == NULL) {
                safuLogErr("String duplication for failed.");
            } else {
                *to = newString;
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}

static safuResultE_t _getBool(samconfConfig_t const *config, char const *from, bool *to) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfigStatusE_t configResult;
    bool value = false;

    configResult = samconfConfigGetBool(config, from, &value);
    if (configResult == SAMCONF_CONFIG_NOT_FOUND) {
        safuLogErrF("Field is missing in the configuration: '%s'", from);
    } else if (configResult != SAMCONF_CONFIG_OK) {
        safuLogErrValue("samconfConfigGetString failed.", configResult);
    } else {
        *to = value;
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _getUint32(samconfConfig_t const *config, char const *from, uint32_t *to) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfigStatusE_t configResult;
    int32_t value = false;

    configResult = samconfConfigGetInt32(config, from, &value);
    if (configResult == SAMCONF_CONFIG_NOT_FOUND) {
        safuLogErrF("Field is missing in the configuration: '%s'", from);
    } else if (configResult != SAMCONF_CONFIG_OK) {
        safuLogErrValue("samconfConfigGetString failed.", configResult);
    } else if (value < 0) {
        safuLogErrF("Field has invalid value (< 0) in the configuration: '%s'", from);
    } else {
        *to = (uint32_t)value;
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosScannerConfigLoad(elosScannerContextShmem_t *context) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    char const *configPath;

    configPath = SCANNER_SHMEM_CONFIG_PATH "/ShmemFile";
    result = _getString(context->config, configPath, &context->shmemFile);
    if (result == SAFU_RESULT_OK) {
        configPath = SCANNER_SHMEM_CONFIG_PATH "/ShmemCreate";
        result = _getBool(context->config, configPath, &context->shmemCreate);
        if (result == SAFU_RESULT_OK) {
            configPath = SCANNER_SHMEM_CONFIG_PATH "/ShmemLogEntries";
            result = _getUint32(context->config, configPath, &context->shmemLogEntries);
            if (result == SAFU_RESULT_OK) {
                if (context->shmemLogEntries < 1) {
                    safuLogErrF("Field has invalid value (< 1) in the configuration: '%s'", configPath);
                    result = SAFU_RESULT_FAILED;
                } else {
                    configPath = SCANNER_SHMEM_CONFIG_PATH "/ShmemOffset";
                    result = _getUint32(context->config, configPath, &context->shmemOffset);
                    if (result == SAFU_RESULT_OK) {
                        configPath = SCANNER_SHMEM_CONFIG_PATH "/SemFile";
                        result = _getString(context->config, configPath, &context->semFile);
                        if (result == SAFU_RESULT_OK) {
                            configPath = SCANNER_SHMEM_CONFIG_PATH "/SemCreate";
                            result = _getBool(context->config, configPath, &context->semCreate);
                        }
                    }
                }
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        context->shmemDataSize = sizeof(elosEbLogRingBuffer_t) + (sizeof(elosEbLogEntry_t) * context->shmemLogEntries);
    }

    return result;
}
