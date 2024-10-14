// SPDX-License-Identifier: MIT
#pragma once

#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct elosSession {
    int fd;
    bool connected;
} elosSession_t;

const char *elosGetLibraryVersion();

/*******************************************************************
 * Function: elosConnect
 *------------------------------------------------------------------
 * Description:
 *      Establishes connection to elos.
 * Input:
 *      - **addrInfo**: addrinfo struct for the host to connect to
 * Output:
 *      - **session**:  session data structure used by other functions
 * Return:
 *      - `true` for success or `false` on failure
 ******************************************************************/
bool elosConnect(struct addrinfo addrInfo, elosSession_t *session);

/*******************************************************************
 * Function: elosConnectTcpip
 *------------------------------------------------------------------
 * Description:
 *      Establishes connection to elos over tcp/ip.
 * Input:
 *      - **host**:     host address as ipv4 or ipv6, e.g. "192.168.2.1"
 *      - **port**:     port number, e.g. 54321
 * Output:
 *      - **session**:  session data structure used by other functions
 * Return:
 *      - `true` for success or `false` on failure
 ******************************************************************/
bool elosConnectTcpip(const char *host, uint16_t port, elosSession_t *session);
