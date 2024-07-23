// SPDX-License-Identifier: MIT

#include "elos/config/config.h"

#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "safu/common.h"
#include "safu/log.h"

#ifndef STORAGE_LOCATION
#define STORAGE_LOCATION "/var/log/elosd.log"
#endif

#ifndef ELOS_KMSG_FILE
#define ELOS_KMSG_FILE "/dev/kmsg"
#endif

#ifndef ELOS_SYSLOG_PATH
#define ELOS_SYSLOG_PATH "/dev/log"
#endif

safuResultE_t elosConfigLoad(samconfConfig_t **config) {
    const char *location = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    safuResultE_t result = SAFU_RESULT_FAILED;

    location = safuGetEnvOr("ELOS_CONFIG_PATH", ELOSD_CONFIG_FILE);
    safuLogInfoF("Use elosd config %s", location);

    status = samconfLoad(location, false, config);
    if (status == SAMCONF_CONFIG_OK) {
        result = SAFU_RESULT_OK;
    }

    return result;
}

static inline int32_t elosConfigGetElosdOptionInt(const samconfConfig_t *config, const char *pathToConfigOption,
                                                  const char *envVarName, int32_t defaultValue) {
    bool validValNotPresent = true;
    bool useEnv = false;
    int32_t optionsFromFile = 0;
    int32_t result = defaultValue;
    samconfConfigStatusE_t exists = {0};

    samconfConfigGetBool(config, ELOS_CONFIG_ROOT "UseEnv", &useEnv);

    if (useEnv) {
        const char *getEnvValue = safuGetEnvOr(envVarName, NULL);
        char *endPtr;
        if (getEnvValue) {
            errno = 0;
            result = strtol(getEnvValue, &endPtr, 10);
            validValNotPresent = endPtr == getEnvValue || errno == ERANGE || errno == EINVAL;
        }
    }

    if (validValNotPresent) {
        exists = samconfConfigGetInt32(config, pathToConfigOption, &optionsFromFile);
        result = exists == SAMCONF_CONFIG_OK ? optionsFromFile : defaultValue;
    }
    return result;
}

static inline const char *elosConfigGetElosdOptionString(const samconfConfig_t *config, const char *pathToConfigOption,
                                                         const char *envVarName, const char *defaultValue) {
    bool useEnv = false;
    const char *optionsFromFile = NULL;
    const char *result = NULL;

    samconfConfigGetBool(config, ELOS_CONFIG_ROOT "UseEnv", &useEnv);
    samconfConfigGetString(config, pathToConfigOption, &optionsFromFile);

    if (useEnv) {
        result = safuGetEnvOr(envVarName, optionsFromFile ? optionsFromFile : defaultValue);
    } else {
        result = optionsFromFile ? optionsFromFile : defaultValue;
    }
    return result;
}

const char *elosConfigGetElosdLogFilter(const samconfConfig_t *config) {
    return elosConfigGetElosdOptionString(config, ELOS_CONFIG_ROOT "LogFilter", "ELOS_LOG_FILTER", ELOSD_LOG_FILTER);
}

safuLogLevelE_t elosConfigGetElosdLogLevel(const samconfConfig_t *config) {
    bool useEnv = false;
    const char *logLevelString = NULL;
    safuLogLevelE_t result = SAFU_LOG_LEVEL_INVALID;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    samconfConfigGetBool(config, ELOS_CONFIG_ROOT "UseEnv", &useEnv);
    if (useEnv) {
        logLevelString = getenv("ELOS_LOG_LEVEL");
        if (logLevelString != NULL) {
            result = safuLogLevelFromString(logLevelString);
            if (result == SAFU_LOG_LEVEL_INVALID) {
                safuLogErrF("reading from env failed! \"%s\" is not a valid log level", logLevelString);
            }
        }
    }
    if (result == SAFU_LOG_LEVEL_INVALID) {
        status = samconfConfigGetString(config, ELOS_CONFIG_ROOT "LogLevel", &logLevelString);
        result = safuLogLevelFromString(logLevelString);
        if (result == SAFU_LOG_LEVEL_INVALID && status == SAMCONF_CONFIG_OK) {
            safuLogErrF("reading from config failed! \"%s\" is not a valid log level", logLevelString);
        }
    }
    if (result == SAFU_LOG_LEVEL_INVALID) {
        result = safuLogLevelFromString(ELOSD_LOG_LEVEL);
    }
    return result;
}

const char *elosConfigGetElosdStorageBackendJsonFile(const samconfConfig_t *config) {
    return elosConfigGetElosdOptionString(config, ELOS_CONFIG_STORAGEBACKEND "Json/File",
                                          "ELOS_STORAGE_BACKEND_JSON_FILE", STORAGE_LOCATION);
}

const char *elosConfigGetElosdScannerPath(const samconfConfig_t *config) {
    return elosConfigGetElosdOptionString(config, ELOS_CONFIG_SCANNER "PluginSearchPath", "ELOS_SCANNER_PATH",
                                          ELOSD_SCANNER_PATH);
}

const char *elosConfigGetElosdClientPath(const samconfConfig_t *config) {
    return elosConfigGetElosdOptionString(config, ELOS_CONFIG_CLIENTINPUTS "PluginSearchPath", "ELOS_CLIENT_PATH",
                                          ELOSD_CLIENT_PATH);
}

const char *elosConfigGetElosdBackendPath(const samconfConfig_t *config) {
    return elosConfigGetElosdOptionString(config, ELOS_CONFIG_EVENTLOGGING "PluginSearchPath", "ELOS_BACKEND_PATH",
                                          ELOSD_BACKEND_PATH);
}

const char *elosConfigGetElosdScannerKmsgFile(const samconfConfig_t *config) {
    return elosConfigGetElosdOptionString(config, ELOS_CONFIG_SCANNER "KmsgScanner/KmsgFile", "ELOS_KMSG_FILE",
                                          ELOS_KMSG_FILE);
}

const char *elosConfigGetElosdSyslogSocketPath(const samconfConfig_t *config) {
    return elosConfigGetElosdOptionString(config, ELOS_CONFIG_SCANNER "SyslogScanner/SyslogPath", "ELOS_SYSLOG_PATH",
                                          ELOS_SYSLOG_PATH);
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
