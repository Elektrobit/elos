// SPDX-License-Identifier: MIT

#include "elos/config/config.h"

#include <libgen.h>
#include <safu/defines.h>
#include <safu/result.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "safu/common.h"
#include "safu/log.h"

#ifndef ELOSD_RUN_DIR
#define ELOSD_RUN_DIR "/run/elosd"
#endif

static char *_concatPath(const char *prefix, const char *name) {
    char *configPath = malloc(strlen(prefix) + strlen(name) + 1);
    if (configPath == NULL) {
        return NULL;
    }
    configPath[0] = 0;
    strcat(configPath, prefix);
    strcat(configPath, name);
    return configPath;
}

static safuResultE_t elosGetConfigPaths(char **baseConfig, char *elosConfigPath) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (elosConfigPath == NULL) {
        safuLogErr("missing Config path");
        result = SAFU_RESULT_FAILED;
    }
    struct stat configPathStat;
    if (result == SAFU_RESULT_OK) {
        if (stat(elosConfigPath, &configPathStat) != 0) {
            safuLogErrF("failed stat on %s", elosConfigPath);
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        switch (configPathStat.st_mode & S_IFMT) {
            case S_IFREG:
                *baseConfig = strdup(elosConfigPath);
                if (*baseConfig != NULL) {
                    elosConfigPath = dirname(elosConfigPath);
                    safuLogWarnF("DEPRECATED: use ELOS_CONFIG_PATH=\"%s\" and \"elosd.json\" as config file name",
                                 elosConfigPath);
                } else {
                    safuLogErr("strdup failed");
                    result = SAFU_RESULT_FAILED;
                }
                break;
            case S_IFDIR:
                *baseConfig = _concatPath(elosConfigPath, "/elosd.json");
                break;
            default:
                safuLogErrF("config %s is neither a config file nor a directory where configuration files exist",
                            elosConfigPath);
                result = SAFU_RESULT_NOT_FOUND;
        }
    }
    return result;
}

static safuResultE_t elosLoadSystemConfigs(samconfConfig_t **config, const char *elosConfigPath,
                                           bool enforceSignature) {
    safuResultE_t result = SAFU_RESULT_OK;

    samconfConfigLocation_t configs[] = {
        CONF_PATH(_concatPath(elosConfigPath, "/elos.d/"), enforceSignature),
        CONF_PATH(_concatPath(elosConfigPath, "/client.d/"), enforceSignature),
        CONF_PATH(_concatPath(elosConfigPath, "/scanner.d/"), enforceSignature),
        CONF_PATH(_concatPath(elosConfigPath, "/eventlogging.d/"), enforceSignature),
    };
    for (size_t i = 0; i < ARRAY_SIZE(configs); i++) {
        safuLogInfoF("+ %s", configs[i].path);
    }
    samconfConfigStatusE_t status = samconfLoadAndMerge(configs, ARRAY_SIZE(configs), config);
    if (status == SAMCONF_CONFIG_INVALID_SIGNATURE) {
        result = SAFU_RESULT_FAILED;
        safuLogErr("at least one config has an invalid signature");
    } else if (status == SAMCONF_CONFIG_ERROR) {
        result = SAFU_RESULT_FAILED;
        safuLogErr("failed loading additional configurations");
    }
    for (size_t i = 0; i < ARRAY_SIZE(configs); i++) {
        free(configs[i].path);
    }

    return result;
}

static safuResultE_t elosLoadEnvironmentVariableConfig(samconfConfig_t **config, bool enforceSignature) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("Using configuration from environment variables");
    samconfConfigLocation_t conf = CONF_PATH("env://?envPrefix=elos&", enforceSignature);
    samconfConfigStatusE_t status = samconfLoadAndMerge(&conf, 1, config);
    if (status != SAMCONF_CONFIG_OK) {
        result = SAFU_RESULT_FAILED;
    }
    return result;
}

safuResultE_t elosConfigLoad(samconfConfig_t **config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    bool enforceSignature = false;
    bool useEnv = false;

    char *baseConfigPath = NULL;
    char *configPath = (char *)safuGetEnvOr("ELOS_CONFIG_PATH", ELOSD_CONFIG_PATH);
    safuResultE_t result = elosGetConfigPaths(&baseConfigPath, configPath);

    if (result == SAFU_RESULT_OK) {
        safuLogInfoF("Base config: %s", baseConfigPath);
        status = samconfLoad(baseConfigPath, enforceSignature, config);
        if (status != SAMCONF_CONFIG_OK) {
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        useEnv = elosConfigGetElosdUseEnv(*config);
        result = elosLoadSystemConfigs(config, configPath, enforceSignature);
    }

    if (result == SAFU_RESULT_OK && useEnv) {
        result = elosLoadEnvironmentVariableConfig(config, enforceSignature);
    }

    free(baseConfigPath);
    if (result != SAFU_RESULT_OK) {
        samconfConfigDelete(*config);
    }
    return result;
}

const char *elosConfigGetElosdLogFilter(const samconfConfig_t *config) {
    return samconfConfigGetStringOr(config, ELOS_CONFIG_ROOT "LogFilter", ELOSD_LOG_FILTER);
}

safuLogLevelE_t elosConfigGetElosdLogLevel(const samconfConfig_t *config) {
    safuLogLevelE_t result = SAFU_LOG_LEVEL_INVALID;

    const char *logLevelString = samconfConfigGetStringOr(config, ELOS_CONFIG_ROOT "LogLevel", ELOSD_LOG_LEVEL);
    result = safuLogLevelFromString(logLevelString);
    if (result == SAFU_LOG_LEVEL_INVALID) {
        safuLogErrF("reading from config failed! \"%s\" is not a valid log level", logLevelString);
    }
    return result;
}

const char *elosConfigGetElosdScannerPath(const samconfConfig_t *config) {
    return samconfConfigGetStringOr(config, ELOS_CONFIG_SCANNER "PluginSearchPath", ELOSD_SCANNER_PATH);
}

const char *elosConfigGetElosdClientPath(const samconfConfig_t *config) {
    return samconfConfigGetStringOr(config, ELOS_CONFIG_CLIENTINPUTS "PluginSearchPath", ELOSD_CLIENT_PATH);
}

const char *elosConfigGetElosdBackendPath(const samconfConfig_t *config) {
    return samconfConfigGetStringOr(config, ELOS_CONFIG_EVENTLOGGING "PluginSearchPath", ELOSD_BACKEND_PATH);
}

const char *elosConfigGetElosdRunDir(const samconfConfig_t *config) {
    return samconfConfigGetStringOr(config, ELOS_CONFIG_ROOT "RunDir", ELOSD_RUN_DIR);
}

bool elosConfigGetElosdUseEnv(const samconfConfig_t *config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    bool useEnv = false;

    status = samconfConfigGetBool(config, ELOS_CONFIG_ROOT "UseEnv", &useEnv);

    if (status != SAMCONF_CONFIG_OK) {
        useEnv = false;
    }

    return useEnv;
}
