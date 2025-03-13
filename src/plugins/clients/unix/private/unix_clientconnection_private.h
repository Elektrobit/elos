// SPDX-License-Identifier: MIT

#pragma once

#include <elos/libelosplugin/connectionmanager.h>
#include <safu/log.h>

/********************************************************************
 * Initializes the given Unix client connection, allocating and
 * preparing all necessary resources.
 *
 * Parameters:
 *     connection : client connection
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ********************************************************************/
safuResultE_t elosUnixClientConnectionInitialize(elosClientConnection_t *connection);

/********************************************************************
 * Destroys the given Unix client connection and freeing all associated
 * resources.
 *
 * Parameters:
 *     connection : client connection
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ********************************************************************/
safuResultE_t elosUnixClientConnectionDelete(elosClientConnection_t *connection);

/********************************************************************
 * Closes the specified client connection, closing any associated
 * resources (e.g., network sockets) and preventing further use of
 * this connection.
 *
 * Parameters:
 *     connection : client connection
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ********************************************************************/
safuResultE_t elosUnixClientConnectionClose(elosClientConnection_t *connection);