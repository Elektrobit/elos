// SPDX-License-Identifier: MIT
#pragma once

#include <elos/event/event.h>
#include <elos/libelos/libelos.h>

#include <string>

/*******************************************************************
 * Function: elosConnectTcpip
 *------------------------------------------------------------------
 * The Elos public interface namespace.
 ******************************************************************/
namespace elos {

/*******************************************************************
 * Result codes for elos operations.
 *
 * Members:
 *     ELOS_RESULT_FAILED    : Operation failed.
 *     ELOS_RESULT_OK        : Operation succeeded.
 *     ELOS_RESULT_NOT_FOUND : Specified item was not found.
 *     ELOS_RESULT_CLOSED    : Operation could not be completed because it is closed.
 ******************************************************************/
enum elosResultE {
    ELOS_RESULT_FAILED = -1,
    ELOS_RESULT_OK = 0,
    ELOS_RESULT_NOT_FOUND = 1,
    ELOS_RESULT_CLOSED = 2,
};

/*******************************************************************
 * Parsed uri string
 *
 * Members:
 *     scheme : scheme specification for identifiers
 *     host :  host address
 *     port: host port
 *     path : path to resource
 *     query : non hierarchical data for identifying resource in uri scope
 *     fragment : identifier for identifying secondary resources in uri
 ******************************************************************/
struct elosUri {
    std::string scheme;
    std::string host;
    std::string port;
    std::string path;
    std::string query;
    std::string fragment;
};

/*******************************************************************
 * Function: elosParseUri
 *------------------------------------------------------------------
 * Function to parse given uri into its components
 * Parameters:
 *      - uri:      uri string with uri components
 * Return:
 *      - elosUri:  uri component struct
 ******************************************************************/
elosUri elosParseUri(const std::string &uri);

/*******************************************************************
 * Elos API class
 *
 * Members:
 *     elosHost : elos ip address
 *     elosPort :  host address
 *     session : session with connection status and connection socket fd
 ******************************************************************/
class Elos {
   public:
    /*******************************************************************
     * Constructor: Elos
     *------------------------------------------------------------------
     * Sets default host and port values to members
     ******************************************************************/
    Elos();

    /*******************************************************************
     * Parameterized Constructor: Elos
     *------------------------------------------------------------------
     * Sets given host and port values members.
     * Parameters:
     *     - host: The elos host address.
     *     - port: The elos host port.
     ******************************************************************/
    Elos(const std::string &host, uint16_t port);

    /*******************************************************************
     * Parameterized Constructor: Elos Uri
     *------------------------------------------------------------------
     * Parse uri string to set host and port members
     * Parameters:
     *     - uri: The uri string containing elos host and port.
     ******************************************************************/
    Elos(const std::string &uri);

    // Prevents copy construction
    Elos(const Elos &) = delete;

    /*******************************************************************
     * Move Constructor: Elos Uri
     *------------------------------------------------------------------
     * Move a existing elos object to another
     ******************************************************************/
    Elos(Elos &&other);

    /*******************************************************************
     * Destructor
     *------------------------------------------------------------------
     * Clean up elos object resources
     ******************************************************************/
    ~Elos();

    elosResultE connect() noexcept;
    elosResultE disconnect() noexcept;
    elosResultE publish(const elosEvent_t *event);
    elosResultE subscribe(char const **filterRuleArray, size_t filterRuleArraySize, elosEventQueueId_t *eventQueueId);

   protected:
    std::string elosHost;
    uint16_t elosPort;
    elosSession_t session;
};
}  // namespace elos
