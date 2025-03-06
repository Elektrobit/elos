// SPDX-License-Identifier: MIT

#pragma once

#include <samconf/samconf.h>
#include <sys/un.h>

#include "elos/libelosplugin/types.h"

/**************************************************************************
 * Get the socket path to be used for this plugin instance.
 *
 * Parameters:
 *      plugin : plugin instance
 * Returns:
 *      - in any case a path to an unix socket.
 *      - compile definition ELOSD_SOCKET_PATH ("/run/elosd/elosd.socket") on failure
 **************************************************************************/
const char *elosUnixConfigGetPath(elosPlugin_t const *plugin);

/**************************************************************************
 * Get the sockaddr to be used for this plugin instance. It contains
 * the path to the unix socket and the address family.
 *
 * Parameters:
 *      plugin : plugin instance
 *      addr   : pointer to a sockaddr_un struct
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 **************************************************************************/
safuResultE_t elosUnixConfigGetSocketAddress(elosPlugin_t const *plugin, struct sockaddr_un *addr);

/********************************************************************
 * Get the connection limit for this plugin instance.
 *
 * Parameters:
 *      plugin : plugin instance
 * Returns:
 *      - in any case a number of max parallel client connections
 *      - compile definition ELOSD_CONNECTION_LIMIT (200) on failure
 ********************************************************************/
int elosUnixConfigGetConnectionLimit(elosPlugin_t const *plugin);
