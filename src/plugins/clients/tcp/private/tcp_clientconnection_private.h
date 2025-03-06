// SPDX-License-Identifier: MIT

#pragma once

#include <elos/libelosplugin/connectionmanager.h>
#include <safu/log.h>

/********************************************************************
 * Initializes the given TCP client connection, allocating and
 * preparing all necessary resources.
 *
 * Parameters:
 *     connection : client connection
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ********************************************************************/
safuResultE_t elosTcpClientConnectionInitialize(elosClientConnection_t *connection);

/********************************************************************
 * Destroys the given TCP client connection and freeing all associated
 * resources.
 *
 * Parameters:
 *     connection : client connection
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ********************************************************************/
safuResultE_t elosTcpClientConnectionDelete(elosClientConnection_t *connection);

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
safuResultE_t elosTcpClientConnectionClose(elosClientConnection_t *connection);