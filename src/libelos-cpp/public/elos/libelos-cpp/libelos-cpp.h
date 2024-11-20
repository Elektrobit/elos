// SPDX-License-Identifier: MIT
#pragma once

#include <elos/event/event.h>
#include <elos/libelos/libelos.h>

#include <string>
#include <vector>

/*******************************************************************
 * elos namespace
 *
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
 *
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
 * Function to parse given uri into its components
 *
 * Parameters:
 *      - uri:      uri string with uri components
 * Return:
 *      - elosUri:  uri component struct
 ******************************************************************/
elosUri elosParseUri(const std::string &uri);

/*******************************************************************
 * Elos event class
 *
 * Public Members:
 *     event : Representation of an elos event
 ******************************************************************/
class Event {
   public:
    elosEvent_t event{};

    /*******************************************************************
     * Constructor: Event
     *
     * Allocates a new elos event
     ******************************************************************/
    Event();

    /*******************************************************************
     * Constructor for Event where event members are provided as arguments
     *
     * Parameters:
     *     - newEvent: The plain C Representation of an elos event.
     ******************************************************************/
    Event(elosEvent_t &&newEvent);

    /*******************************************************************
     * Parameterized Constructor: Event
     *------------------------------------------------------------------
     * Representation of an elos event.
     * Parameters:
     *     - date: The unix timestamp in nano seconds resolution.
     *     - source: A struct containing informations about where the event originated from.
     *     - severity:The possible severities for an Event.
     *     - hardwareid: An unique identifier for the hardware.
     *     - classification: The classification attribute is used to categorize events.
     *     - messageCode: Provide information without text in payload.
     *     - payload: The actual payload of the information.
     ******************************************************************/
    Event(struct timespec date, elosEventSource_t &source, elosSeverityE_t severity, const std::string &hardwareid,
          uint64_t classification, elosEventMessageCodeE_t messageCode, const std::string &payload);

    /*******************************************************************
     * Constructor for Event where member source is passed using the move
     * semantic and other members are provided as arguments
     *
     * Parameters:
     *     - date: The unix timestamp in nano seconds resolution.
     *     - source: rvalue of source
     *     - severity:The possible severities for an Event.
     *     - hardwareid: An unique identifier for the hardware.
     *     - classification: The classification attribute is used to categorize events.
     *     - messageCode: Provide information without text in payload.
     *     - payload: The actual payload of the information.
     ******************************************************************/
    Event(struct timespec date, elosEventSource_t &&source, elosSeverityE_t severity, const std::string &hardwareid,
          uint64_t classification, elosEventMessageCodeE_t messageCode, const std::string &payload);

    /*******************************************************************
     * Destructor to clean up elos object resources and deallocates event.
     *
     ******************************************************************/
    ~Event();
};

/*******************************************************************
 * Subscription class
 *
 * Members:
 *     subscription : struct containing queue id
 *     filter :  filter rule string to filter subscribed events.
 ******************************************************************/
class Subscription {
   protected:
    elosSubscription_t subscription;

   public:
    /*******************************************************************
     * default empty constructor.
     *
     ******************************************************************/
    Subscription() = default;

    /*******************************************************************
     * default move constructor.
     *
     ******************************************************************/
    Subscription(const Subscription &subscription) = default;

    /*******************************************************************
     * default empty destructor.
     *
     ******************************************************************/
    ~Subscription() = default;

    friend class Elos;
};

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
     * Constructor to set default host and port values to members
     *
     ******************************************************************/
    Elos();

    /*******************************************************************
     * Parameterized Constructor to set given host and port values members.
     *
     * Parameters:
     *     - host: The elos host address.
     *     - port: The elos host port.
     ******************************************************************/
    Elos(const std::string &host, uint16_t port);

    /*******************************************************************
     * Parameterized Constructor to parse uri string to set host and
     * port members
     * Parameters:
     *     - uri: The uri string containing elos host and port.
     ******************************************************************/
    Elos(const std::string &uri);

    // Prevents copy construction
    Elos(const Elos &) = delete;

    /*******************************************************************
     * Move Constructor to move a existing elos object to another.
     *
     * Parameters:
     *     - other: existing elos object.
     ******************************************************************/
    Elos(Elos &&other);

    /*******************************************************************
     * Destructor to clean up elos object resources
     *
     ******************************************************************/
    ~Elos();

    /*******************************************************************
     * Function to publish given reference to event
     *
     * Info:
     *      publish will try to establish a connection with elosd, if current
     *      class instance is not connected to elosd
     *
     * Parameters:
     *      - event:        event to published by elos client
     *      - shallConnect: if instance is not connected but tries to publish, this
     *                      parameters shall inform if the instance should try to
     *                      connect or not.
     * Return:
     *      - elosResultE:  ELOS_RESULT_OK - if event is published successfully
     *                      ELOS_RESULT_FAILED - if invalid session or publish fails
     ******************************************************************/
    elosResultE publish(const Event &event, bool shallConnect);

    /*******************************************************************
     * Function to publish an event using move semantic
     *
     * Info:
     *      publish will try to establish a connection with elosd, if current
     *      class instance is not connected to elosd
     *
     * Parameters:
     *      - event:     event to published by elos client
     *      - shallConnect: if instance is not connected but tries to publish, this
     *                      parameters shall inform if the instance should try to
     *                      connect or not.
     * Return:
     *      - elosResultE:  ELOS_RESULT_OK - if event is published successfully
     *                      ELOS_RESULT_FAILED - if invalid session or publish fails
     ******************************************************************/
    elosResultE publish(Event &&event, bool shallConnect);

    /*******************************************************************
     * Overloading the insertion operator to publish given event.
     *
     * Info:
     *      publish will try to establish a connection with elosd, if current
     *      class instance is not connected to elosd
     *
     * Parameter:
     *      - event:     event to published by elos client
     ******************************************************************/
    void operator<<(const Event &event);

    /*******************************************************************
     * Overloading the insertion operator to publish event using move semantic
     *
     * Info:
     *      publish will try to establish a connection with elosd, if current
     *      class instance is not connected to elosd
     *
     * Parameter:
     *      - event:     event to published by elos client
     ******************************************************************/
    void operator<<(Event &&event);

    /*******************************************************************
     * Function to connect to elos
     *
     * Parameters:
     *
     * Return:
     *      - elosResultE: Returns whether connection attempt was successful
     ******************************************************************/
    elosResultE connect();

    /*******************************************************************
     * Function to disconnect from elos
     *
     * Parameters:
     *
     * Return:
     *      - elosResultE: Returns whether disconnect attempt was successful
     ******************************************************************/
    elosResultE disconnect();

    /*******************************************************************
     * Function: subscribe
     *------------------------------------------------------------------
     * Function to subscribe to events matching the filter.
     * Parameters:
     *      - filter:     filter string to subcribe to.
     * Return:
     *      - Subscription:  subscription object with filter added and elosSubscription member
     ******************************************************************/
    Subscription subscribe(std::string filter);

    /*******************************************************************
     * Function to read subscripted Events.
     *
     * Parameters:
     *      - subscription: subscription object with filter added and elosSubscription
     * Return:
     *      - Event:        vector of new subscripted events
     ******************************************************************/
    std::vector<elos::Event> read(Subscription &subscription);

   protected:
    std::string elosHost;
    uint16_t elosPort;
    elosSession_t session;
};
}  // namespace elos
