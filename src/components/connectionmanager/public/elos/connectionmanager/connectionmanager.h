// SPDX-License-Identifier: MIT
#pragma once

#include "elos/connectionmanager/types.h"

/*******************************************************************
 * Initializes a new ConnectionManager.
 *
 * Parameters:
 *      connectionManager: ConnectionManager structure that will be initialized
 *      param: Parameters for initialization. Also see connectionmanager_types.h
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosConnectionManagerInitialize(elosConnectionManager_t *connectionManager,
                                              elosConnectionManagerParam_t *param);

/*******************************************************************
 * Frees member resources used by the given ConnectionManager data structure.
 * Will also automatically stop an active worker thread before
 * freeing up the used resources.
 *
 * Parameters:
 *      connectionManager: ConnectionManager whose members shall be freed
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosConnectionManagerDeleteMembers(elosConnectionManager_t *connectionManager);

/*******************************************************************
 * Creates a worker thread waiting for incoming connections.
 * Each ConnectionManager can only handle one worker thread,
 * calling Start a second time will result in an error.
 *
 * Parameters:
 *      connectionManager: ConnectionManager structure to started
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosConnectionManagerStart(elosConnectionManager_t *connectionManager);

/*******************************************************************
 * Stops a currently active worker thread.
 *
 * Parameters:
 *      connectionManager: ConnectionManager structure to stopped
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosConnectionManagerStop(elosConnectionManager_t *connectionManager);
