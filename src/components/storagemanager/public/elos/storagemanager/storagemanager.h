// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

#include "elos/storagemanager/storagemanager_types.h"
#include "elos/pluginmanager/pluginmanager.h"

/**
 * Initialize the storage manager with default values to be safely used.
 *
 * Parameters:
 *     storageManager (elosStorageManager_t*): storage manager component initialized with storage pluginmanager
 *     param (elosStorageManagerParam_t *) : parameter used to initialize storage manager component. See
 *     `storagemanagertypes.h`
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosStorageManagerInitialize(elosStorageManager_t *storageManager, elosStorageManagerParam_t const *param);

/**
 * Start storage manager with storage plugins
 *
 * Parameters:
 *     storageManager (elosStorageManager_t*): storage manager with storage plugins is started.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosStorageManagerStart(elosStorageManager_t *storageManager);

/**
 * Stop a running storage manager instance.
 *
 * Parameters:
 *     storageManager (elosStorageManager_t*): storage manager to be stopped.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosStorageManagerStop(elosStorageManager_t *storageManager);

/**
 * Shall cleanup and free all members of `elosStorageManagert_t`.
 *
 * Parameters:
 *     storageManager (elosStorageManager_t*): storage manager component to be freed.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosStorageManagerDeleteMembers(elosStorageManager_t *storageManager);
