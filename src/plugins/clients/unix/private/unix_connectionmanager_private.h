// SPDX-License-Identifier: MIT

#pragma once

#include <elos/libelosplugin/connectionmanager.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/log.h>

/********************************************************************
 * Initializes the given Unix connection manager, allocating and
 * preparing all necessary resources. Also initializes any plugin-specific
 * function pointers needed by the connection manager.
 *
 * Parameters:
 *     connectionManager: pointer to where the new manager instance will be allocated and returned
 *     plugin : plugin instance
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ********************************************************************/
safuResultE_t elosUnixConnectionManagerNew(elosConnectionManager_t **connectionManager, elosPlugin_t *plugin);

/********************************************************************
 * Destroys the given Unix connection manager and freeing all associated
 * resources.
 *
 * Parameters:
 *     connectionManager: connection manager to allocate
 *     plugin : plugin instance
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 * ********************************************************************/
safuResultE_t elosUnixConnectionManagerDelete(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin);
