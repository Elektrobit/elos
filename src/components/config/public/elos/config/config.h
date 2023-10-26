// SPDX-License-Identifier: MIT
#ifndef ELOS_CONFIG_H
#define ELOS_CONFIG_H

#include <safu/log.h>
#include <samconf/samconf.h>

#include "elos/config/defines.h"

int elosConfigLoad(samconfConfig_t **config);
int elosConfigGetElosdPort(const samconfConfig_t *config);
const char *elosConfigGetElosdInterface(const samconfConfig_t *config);
const char *elosConfigGetElosdLogFilter(const samconfConfig_t *config);
safuLogLevelE_t elosConfigGetElosdLogLevel(const samconfConfig_t *config);
const char *elosConfigGetElosdStorageBackendJsonFile(const samconfConfig_t *config);
const char *elosConfigGetElosdScannerPath(const samconfConfig_t *config);
const char *elosConfigGetElosdBackendPath(const samconfConfig_t *config);
const char *elosConfigGetElosdScannerKmsgFile(const samconfConfig_t *config);
const char *elosConfigGetElosdSyslogSocketPath(const samconfConfig_t *config);

#endif
