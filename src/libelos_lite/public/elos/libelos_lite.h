// SPDX-License-Identifier: MIT
#pragma once

#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>

#include "elos/lite/event.h"

/*******************************************************************
 * safes a handle for a connection to elosd
 ******************************************************************/
typedef struct elosliteSession {
    int fd;
    bool connected;
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
 * Establishes connection to elos over unix domain socket.
 *
 * Parameters:
 *      socketPath: complete path to the unix domain socket that shall be created
 *      session: session data structure used to store the connection
 *          for other functions
 * Returns:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool elosliteConnectUnix(const char *socketPath, elosliteSession_t *session);

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

/*******************************************************************
 * Log a formated message to elos
 *
 * Parameters:
 *      session:  session data structure holding the connection
 *      event:    the event to publish
 *      fmtMessage: message that will become the event's payload
 * Return:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool elosliteLogMessageFormated(elosliteSession_t *session, elosSeverityE_t severity, char *fmtMessage, ...);

/*******************************************************************
 * Fill an elos source struct with commonly available values
 *
 * Parameters:
 *      source:  pointer to valid event source struct to be filled
 * Return:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool elosliteFillEventSource(elosliteEventSource_t *source);

/*******************************************************************
 * Retreive the machine ID
 *
 * Parameters:
 *      id:  pointer to pointer of char that will point to the machine ID. Do not free!
 * Return:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool elosliteRetreiveMachineID(char **id);

/*******************************************************************
 * Prepare an elos event.
 *
 * Parameters:
 *      event:  pointer to target event
 *      source: pointer to event source structure
 *      severity: severity
 *      hardwareid: unique hardware ID
 *      classification: classification code
 *      messageCode: elos event message code
 *      payload: free text message
 * Return:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool eloslitePrepareElosEvent(elosliteEvent_t *event, elosliteEventSource_t source, elosSeverityE_t severity, char *hardwareid, uint64_t classification, elosEventMessageCodeE_t messageCode, char *payload);

/*******************************************************************
 * Prepare an elos event.
 *
 * Parameters:
 *      source: pointer to target event source structure
 *      appName: pointer to application name string
 *      appFile: pointer to string with name of application's executable file
 * Return:
 *      `true` for success or `false` on failure
 ******************************************************************/
bool eloslitePrepareEventSource(elosliteEventSource_t *source, char *appName, char *appFile);
