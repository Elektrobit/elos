// SPDX-License-Identifier: MIT
#pragma once

#include "elos/clientmanager/types.h"

/*******************************************************************
 * Initializes a new ClientManager.
 *
 * Parameters:
 *      clientManager: ClientManager structure that will be initialized
 *      param: Parameters for initialization. Also see clientmanager_types.h
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientManager, elosClientManagerParam_t *param);

/*******************************************************************
 * Frees member resources used by the given ClientManager data structure.
 * Will also automatically stop an active worker thread before
 * freeing up the used resources.
 *
 * Parameters:
 *      clientManager: ClientManager whose members shall be freed
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientManager);

/*******************************************************************
 * Creates a worker thread waiting for incoming connections.
 * Each ClientManager can only handle one worker thread,
 * calling Start a second time will result in an error.
 *
 * Parameters:
 *      clientManager: ClientManager structure to started
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientManagerStart(elosClientManager_t *clientManager);

/*******************************************************************
 * Stops a currently active worker thread.
 *
 * Parameters:
 *      clientManager: ClientManager structure to stopped
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientManagerStop(elosClientManager_t *clientManager);
