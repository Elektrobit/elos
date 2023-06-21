// SPDX-License-Identifier: MIT
#ifndef ELOS_CONFIG_H
#define ELOS_CONFIG_H

#include <safu/log.h>
#include <samconf/samconf.h>

#ifndef ELOS_CONFIG_ROOT
#define ELOS_CONFIG_ROOT "/root/elos/"
#endif

#ifndef ELOS_CONFIG_EVENTLOGGING
#define ELOS_CONFIG_EVENTLOGGING ELOS_CONFIG_ROOT "EventLogging/"
#endif

#ifndef ELOS_CONFIG_SCANNER
#define ELOS_CONFIG_SCANNER ELOS_CONFIG_ROOT "Scanner/"
#endif

#ifndef ELOS_CONFIG_STORAGEBACKEND
#define ELOS_CONFIG_STORAGEBACKEND ELOS_CONFIG_ROOT "StorageBackend/"
#endif

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
