// SPDX-License-Identifier: MIT
#pragma once

#include "elos/lite/event.h"

#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct elosliteBuffer {
    char *buffer; // = NULL
    size_t size; // sizeof(buffer)
    size_t pos; // = 0
} elosliteBuffer_t;

/*******************************************************************
 * safes a handle for a connection to elosd
 ******************************************************************/
typedef struct elosliteSession {
    int fd;
    bool connected;
    elosliteBuffer_t buffer;
} elosliteSession_t;

const char *elosliteGetLibraryVersion();

/*******************************************************************
 * Establishes connection to elos.
 *
 * Parameters:
 *      addrInfo: addrinfo struct for the host to connect to
 *      session: session data structure used to store the connection
 *          for other functions
 * Returns:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool elosliteConnect(struct addrinfo addrInfo, elosliteSession_t *session);

/*******************************************************************
 * Establishes connection to elos over tcp/ip.
 *
 * Parameters:
 *      host:     host address as ipv4 or ipv6, e.g. "192.168.2.1"
 *      port:     port number, e.g. 54321
 *      session:  session data structure used by other functions
 * Returns:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool elosliteConnectTcpip(const char *host, uint16_t port, elosliteSession_t *session);

/*******************************************************************
 * Closes a connection to elos.
 *
 * Parameters:
 *      session:  session data structure holding the connection
 * Return:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool elosliteDisconnect(elosliteSession_t *session);

/*******************************************************************
 * Publish an event to elos.
 *
 * Parameters:
 *      session:  session data structure holding the connection
 *      event:    the event to publish
 * Return:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool eloslitePublish(elosliteSession_t *session, elosliteEvent_t *event);
