// SPDX-License-Identifier: MIT

#include "shmem_config.h"

#include <elos/libelosplugin/types.h>
#include <safu/common.h>
#include <safu/log.h>
#include <samconf/samconf.h>

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

safuResultE_t elosScannerConfigLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosScannerContextShmem_t *context = plugin->data;
    char const *configPath;

    result = _getString(plugin->config, "Config/ShmemFile", &context->shmemFile);
    if (result == SAFU_RESULT_OK) {
        result = _getBool(plugin->config, "Config/ShmemCreate", &context->shmemCreate);
        if (result == SAFU_RESULT_OK) {
            configPath = "Config/ShmemLogEntries";
            result = _getUint32(plugin->config, configPath, &context->shmemLogEntries);
            if (result == SAFU_RESULT_OK) {
                if (context->shmemLogEntries < 1) {
                    safuLogErrF("Field has invalid value (< 1) in the configuration: '%s'", configPath);
                    result = SAFU_RESULT_FAILED;
                } else {
                    result = _getUint32(plugin->config, "Config/ShmemOffset", &context->shmemOffset);
                    if (result == SAFU_RESULT_OK) {
                        result = _getString(plugin->config, "Config/SemFile", &context->semFile);
                        if (result == SAFU_RESULT_OK) {
                            result = _getBool(plugin->config, "Config/SemCreate", &context->semCreate);
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
