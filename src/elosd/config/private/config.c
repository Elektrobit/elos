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

safuResultE_t elosConfigLoad(samconfConfig_t **config) {
    char *configPath = NULL;
    char *pathBuffer = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool enforceSignature = false;
    bool useEnv = false;

    configPath = (char *)safuGetEnvOr("ELOS_CONFIG_PATH", ELOSD_CONFIG_PATH);
    struct stat configPathStat;
    if (stat(configPath, &configPathStat) != 0) {
        safuLogErrF("failed stat on %s", configPath);
        return SAFU_RESULT_FAILED;
    }
    const char *location = NULL;
    switch (configPathStat.st_mode & S_IFMT) {
        case S_IFREG:
            status = samconfLoad(configPath, enforceSignature, config);
            safuLogInfoF("Base config %s", configPath);
            pathBuffer = strdup(configPath);
            if (pathBuffer != NULL) {
                configPath = dirname(pathBuffer);
                safuLogWarnF("deprecated: use ELOS_CONFIG_PATH=\"%s\" and \"elosd.json\" as config file name",
                             configPath);
            } else {
                safuLogErr("strdup failed");
                if (status == SAMCONF_CONFIG_OK) {
                    samconfConfigDelete(*config);
                }
                status = SAMCONF_CONFIG_ERROR;
                result = SAFU_RESULT_FAILED;
            }
            break;
        case S_IFDIR:
            location = _concatPath(configPath, "/elosd.json");
            status = samconfLoad(location, enforceSignature, config);
            safuLogInfoF("Base config %s", location);
            free((void *)location);
            break;
        default:
            safuLogErrF("config %s is neigther a config file nor a directory where to find configs", configPath);
            return SAFU_RESULT_FAILED;
    }

    if (status == SAMCONF_CONFIG_OK) {
        result = SAFU_RESULT_OK;
        useEnv = elosConfigGetElosdUseEnv(*config);
        samconfConfigLocation_t configs[] = {
            CONF_PATH(_concatPath(configPath, "/elos.d/"), enforceSignature),
            CONF_PATH(_concatPath(configPath, "/client.d/"), enforceSignature),
            CONF_PATH(_concatPath(configPath, "/scanner.d/"), enforceSignature),
            CONF_PATH(_concatPath(configPath, "/eventlogging.d/"), enforceSignature),
        };
        for (size_t i = 0; i < ARRAY_SIZE(configs); i++) {
            safuLogInfoF("+ %s", configs[i].path);
        }

        status = samconfLoadAndMerge(configs, ARRAY_SIZE(configs), config);
        if (status == SAMCONF_CONFIG_OK) {
            result = SAFU_RESULT_OK;
        } else {
            safuLogDebug("Nothing loaded from additional configs");
        }
        for (size_t i = 0; i < ARRAY_SIZE(configs); i++) {
            free(configs[i].path);
        }

        if (result == SAFU_RESULT_OK) {
            if (useEnv) {
                samconfConfigLocation_t conf = CONF_PATH("env://?envPrefix=elos&", enforceSignature);
                status = samconfLoadAndMerge(&conf, 1, config);
                if (status != SAMCONF_CONFIG_OK) {
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    } else {
        safuLogErr("in loading base config file");
    }

    free(pathBuffer);
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
