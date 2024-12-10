// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

#include "elos/clientmanager/clientmanager_types.h"
#include "elos/pluginmanager/pluginmanager.h"

__BEGIN_DECLS

/**
 * Initialize the client manager with default values to be safely used.
 *
 * Parameters:
 *     clientManager (elosClientManager_t*): client manager component initlaized with client pluginmanager
 *     param (elosClientManagerParam_t *) : parameter used to initialize client manager component. See
 *     clientmanager_types.h
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientManager, elosClientManagerParam_t const *param);

/**
 * Start client manager with client plugins
 *
 * Parameters:
 *     clientManager (elosClientManager_t*): client manager with client plugins is started.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */

safuResultE_t elosClientManagerStart(elosClientManager_t *clientManager);

/**
 * Stop a running  client manager instance.
 *
 * Parameters:
 *     clientManager (elosClientManager_t*): client manager to be stopped.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosClientManagerStop(elosClientManager_t *clientManager);

/**
 * Shall cleanup and free all members of elosClientManager_t.
 *
 * Parameters:
 *     clientManager (elosClientManager_t*): client manager component to be freed.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientManager);

__END_DECLS
