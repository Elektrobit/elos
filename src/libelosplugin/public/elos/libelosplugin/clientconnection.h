// SPDX-License-Identifier: MIT
#pragma once
#include <elos/libelosplugin/clientconnection_types.h>
#include <safu/result.h>

__BEGIN_DECLS

/*******************************************************************
 * Initializes a new ClientConnection.
 *
 * Parameters:
 *      clientConnection: ClientConnection structure that will be initialized
 *      sharedData: Reference to shared data of all connecitons.
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientConnectionInitialize(elosClientConnection_t *clientConnection,
                                             elosClientConnectionSharedData_t *sharedData);

/*******************************************************************
 * Creates a worker thread using the given socketFd for communication.
 * Each ClientConnection can only handle one worker thread,
 * calling Start a second time will result in an error.
 *
 * Parameters:
 *      clientConnection: The ClientConnection to be started
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientConnectionStart(elosClientConnection_t *clientConnection);

/*******************************************************************
 * Stops the currently active worker thread.
 * Start can be called again with a new socketFd after
 * a ClientConnection has been stopped.
 *
 * Parameters:
 *      clientConnection: The ClientConnection to be stopped
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientConnectionStop(elosClientConnection_t *clientConnection);

/*******************************************************************
 * Frees all resources used be the given ClientConnection.
 * Will also automatically Stop an active worker thread
 * before freeing up the resources.
 *
 * Parameters:
 *      clientConnection: The ClientConnection to be freed
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientConnectionDeleteMembers(elosClientConnection_t *clientConnection);

/*******************************************************************
 * Checks if the given ClientConnection has been started or not.
 *
 * Parameters:
 *      clientConnection: The ClientConnection to be checked
 *      active: set to 'true' if a connection is active/started, 'false' otherwise.
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ******************************************************************/
safuResultE_t elosClientConnectionIsActive(elosClientConnection_t *clientConnection, bool *active);

/*******************************************************************
 * Worker thread function used by pthread_create()
 *
 * Parameters:
 *      ptr: Pointer to a ClientConnection struct
 * Returns:
 *      - Always NULL
 ******************************************************************/
void *elosClientConnectionWorker(void *ptr);

__END_DECLS
