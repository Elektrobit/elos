// SPDX-License-Identifier: MIT

#pragma once

#include <netinet/in.h>
#include <samconf/samconf.h>

#include "elos/libelosplugin/types.h"

/********************************************************************
 * Get the configured TCP port for this plugin instance.
 *
 * Parameters:
 *      plugin : plugin instance
 * Returns:
 *      - in any case the port to use
 *      - on any failure the compile definition of ELOSD_PORT is used
 ********************************************************************/
int elosTcpConfigGetPort(elosPlugin_t const *plugin);

/********************************************************************
 * Get the connection limit for this plugin instance.
 *
 * Parameters:
 *      plugin : plugin instance
 * Returns:
 *      - in any case a number of max parallel client connections
 *      - compile definition ELOSD_CONNECTION_LIMIT (200) on failure
 ********************************************************************/
int elosTcpConfigGetConnectionLimit(elosPlugin_t const *plugin);

/**************************************************************************
 * Get the interface to be used for this plugin instance. The string could
 * either contain an IP address (V4/V6) or a host name.
 *
 * Parameters:
 *      plugin : plugin instance
 * Returns:
 *      - in any case an IP address or a host name.
 *      - compile definition ELOSD_INTERFACE (200) on failure
 **************************************************************************/
const char *elosTcpConfigGetInterface(elosPlugin_t const *plugin);

/**************************************************************************
 * Get the sockaddr to be used for this plugin instance. It contains
 * an IP address (V4/V6), a host name and the address family.
 *
 * Parameters:
 *      plugin : plugin instance
 *      addr   : pointer to a struct sockaddr_in
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 **************************************************************************/
safuResultE_t elosTcpConfigGetSocketAddress(elosPlugin_t const *plugin, struct sockaddr_in *addr);
