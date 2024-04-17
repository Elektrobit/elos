// SPDX-License-Identifier: MIT
#ifndef ELOS_CONFIG_H
#define ELOS_CONFIG_H

#include <safu/log.h>
#include <safu/result.h>
#include <samconf/samconf.h>

#include "elos/config/defines.h"

__BEGIN_DECLS

safuResultE_t elosConfigLoad(samconfConfig_t **config);
const char *elosConfigGetElosdLogFilter(const samconfConfig_t *config);
safuLogLevelE_t elosConfigGetElosdLogLevel(const samconfConfig_t *config);
const char *elosConfigGetElosdStorageBackendJsonFile(const samconfConfig_t *config);
const char *elosConfigGetElosdScannerPath(const samconfConfig_t *config);
const char *elosConfigGetElosdBackendPath(const samconfConfig_t *config);
const char *elosConfigGetElosdClientPath(const samconfConfig_t *config);
const char *elosConfigGetElosdScannerKmsgFile(const samconfConfig_t *config);
const char *elosConfigGetElosdSyslogSocketPath(const samconfConfig_t *config);

/************************************************************************************
 * Determine if configuration options from environment shall be evaluated or ignored.
 *
 * Parameters:
 *   config: root config containing the `root.elos.useEnv` option 
 *
 * Return:
 *   true: if environment options shall be evaluated
 *   false: in any other case including errors on Reading accessing `config`.
 ***********************************************************************************/
bool elosConfigGetElosdUseEnv(const samconfConfig_t *config);

__END_DECLS

#endif
