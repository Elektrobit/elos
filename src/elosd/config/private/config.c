// SPDX-License-Identifier: MIT

#include "elos/config/config.h"

#include <safu/defines.h>
#include <safu/result.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
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

#ifndef ELOS_RUN_DIR
#define ELOS_RUN_DIR "/run/elosd"
#endif

safuResultE_t elosConfigLoad(samconfConfig_t **config) {
    char *location = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool enforceSignature = false;

    location = (char *)safuGetEnvOr("ELOS_CONFIG_PATH", ELOSD_CONFIG_FILE);
    safuLogInfoF("Use elosd config %s", location);

    samconfConfigLocation_t configs[] = {
        CONF_PATH(location, enforceSignature),
        CONF_PATH("/etc/elos/elos.d/", enforceSignature),
        CONF_PATH("/etc/elos/client.d/", enforceSignature),
        CONF_PATH("/etc/elos/eventlogging.d/", enforceSignature),
        CONF_PATH("/etc/elos/scanner.d/", enforceSignature),
    };
    status = samconfLoadAndMerge(configs, ARRAY_SIZE(configs), config);
    if (status == SAMCONF_CONFIG_OK) {
        result = SAFU_RESULT_OK;
    } else {
        safuLogErr("in config load and merge");
    }

    if (result == SAFU_RESULT_OK) {
        if (elosConfigGetElosdUseEnv(*config)) {
            samconfConfigLocation_t conf = CONF_PATH("env://?envPrefix=elos&", enforceSignature);
            status = samconfLoadAndMerge(&conf, 1, config);
            if (status != SAMCONF_CONFIG_OK) {
                result = SAFU_RESULT_FAILED;
            }
        }
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
    return samconfConfigGetStringOr(config, ELOS_CONFIG_ROOT "RunDir", ELOS_RUN_DIR);
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
