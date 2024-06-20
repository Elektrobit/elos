// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

#include "elos/pluginmanager/pluginmanager.h"
#include "elos/scannermanager/scannermanager_types.h"

__BEGIN_DECLS

/**
 * Initialize the scanner manager with default values to be safely used.
 *
 * Parameters:
 *     scannerManager (elosScannerManager_t*): scanner manager component initialized with scanner pluginmanager
 *     param (elosScannerManagerParam_t *) : parameter used to initialize scanner manager component. See
 *     `scannermanagertypes.h`
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosScannerManagerInitialize(elosScannerManager_t *scannerManager,
                                           elosScannerManagerParam_t const *param);

/**
 * Start scanner manager with scanner plugins
 *
 * Parameters:
 *     scannerManager (elosScannerManager_t*): scanner manager with scanner plugins is started.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosScannerManagerStart(elosScannerManager_t *scannerManager);

/**
 * Stop a running scanner manager instance.
 *
 * Parameters:
 *     scannerManager (elosScannerManager_t*): scanner manager to be stopped.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosScannerManagerStop(elosScannerManager_t *scannerManager);

/**
 * Shall cleanup and free all members of `elosScannerManagert_t`.
 *
 * Parameters:
 *     scannerManager (elosScannerManager_t*): scanner manager component to be freed.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosScannerManagerDeleteMembers(elosScannerManager_t *scannerManager);

__END_DECLS
