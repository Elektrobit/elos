Event Logging
=============

The event logging system implements a data collection facility, which
collects events from different sources to filters and limits them in
order to store them. For details about the canonical event format see
“canonical event format”.

The Event Logging system has the following main goals:

-  filter events depending on filter settings
-  limit events to log if a configured limit (logs per time) is exceeded
-  optimize storage access to minimize IO operations and maximize
   physical storage lifetime

Motivation:

| Event Management and Logging System (elos). A key weakness in many
| operating system solutions is the lack of a consistent, coherent and
| easy-to-use way to handle events, for example program failure or network
| interface status change. Needed: Set of APIs and services that simplify
| all this into a single multipurpose solution. The APIs shall provide an
| easy-to-use method to programmatically generate or consume events, both in real
| time and post-facto from the logs. The logging shall be configured to rate
| limit, filter on many different criteria and store temporarily or permanently

For the design the following assumptions are made: (TBD make some
reliable measurements)

-  the upper limit of available storage is 100GB
-  an storage entity the event is round about <= 100Kb
-  10⁶ events; 100 event/s => 60H
-  All events are treated equal / categorize events and apply different
   storage retention policies
-  What is the minimal expected time frame to hold events (1a ?)
-  512 Bytes is smallest storage entity size (SSD/MMC always write at
   least 512 byte)

|overview event logging|

Log aggregation
---------------


The details on storing events shall be encapsulated by a component
called LogAggregator. This component is responsible for the following
tasks:

-  manage storage backend start, stop
-  filter and limit event logs
-  forward events to the actual storage backend to persist them
-  manage concurrent storage requests
-  manage retention policy, (trigger log cleanup, drop old logs )
-  monitor storage backend health, generate notification about failing
   storage backend
-  manage multiple backends (compile time or as loadable plugin?)

The Loggagregator is a shared instance in the elosd. So it has to
synchronize all attempts to persist an event.




Logging Backend(s)
~~~~~~~~~~~~~~~~~~

The actual storage process depends on the storage engine used. Therefore
this is abstracted in so called Storage backends. A storage backend
takes one or more events store them in the underlying storage engine. A
storage engine could be a simple file write, a database or any other
solution that fits the needs for the target system.

The interface of a storage backend consist basically of the following
tasks:

-  Backend open
-  Backend store
-  Backend close
-  Realize required retention policy

The following Design Decision shall describe the decision process for
the final storage backend to use.

Design Decision – Storage backend
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A) JSON-File based

B) DBMS - SQLITE

C) Time series DB

D) NoSQL

E) Custom implementation


.. |overview event logging| image:: /doc/images/overview_event_logging.png

Fetch API
---------

Requirements:
~~~~~~~~~~~~~

* Data-source not persistent (In-Memory)
* Limits:
  * Last <n> events per source/per msg-code
  * Maybe: msg-counter (how many events of this type)
* Ensure/Respect the ~65KB message-size-limit of the protocol

Static View:
~~~~~~~~~~~~

#. One storage backend provides a read-access to older events to provide the fetchAPI.
#. The fetchAPI-backend (**fab**) is a normal storage backend of elos and is loaded
   and configure as a normal storage backend. Thus the fab only receives events
   that pass the RPN filter configure to that backend.
#. The plugin-name of the fab must be „fetchapi“ to be recognised.
   Thus only one storage-backend-plugin-instance configured with the name "fetchapi" is allowed.
#. Basic feature of fab: one ringbuffer for all events. The size can be configured.
#. Advanced feature of fab: tbd
#. The plug-in is preferred to be written in C++ with a ringbuffer and sync
   from the stdc++.
#. Limit fetch result to maintain 65kb protocol limit by providing a Paging-API
   and indicate to large results with a truncated flag

.. uml::
   :caption: Component overview of the LogAggregator

   node "elosd" {

     package EventLogging {
       collections "StoragePlugins" <<Plugin>>
       component "FetchAPI-backend" as FAB  <<Plugin>>
       component "LogAggregator" as LAG

       LAG ..up..> StoragePlugins : store

       LAG ..up..> FAB : store
       LAG ..up..> FAB : fetch

       fetchAPI -up- LAG  
       storeAPI -up- LAG  
     }

     component "ClientPlugin"
     component "ScannerPlugin"

     ClientPlugin -up-( fetchAPI
     ClientPlugin -up-( storeAPI

     ScannerPlugin -up-( fetchAPI
     ScannerPlugin -up-( storeAPI

   }

.. code-block:: json
   :caption: Example configuration snippet for `fetchAPI` storage backend

   {
      "root": {
        "elos": {
          "EventLogging": {
            "Plugins": {
              "fetchapi": {
                "run": "always",
                "file": "inmemory.so",
                "Filter": [
                  "1 1 EQ"
                ],
                "config": {
                  "size": "1MB"
                }
              }
            }
          }
        }
      }
    }



Dynamic View
~~~~~~~~~~~~


.. uml::
   :caption: The fetch event process

    collections Caller as Caller
    participant fetchAPI as fetchAPI
    database InMemoryStorage as InMemoryStorage


    Caller --> fetchAPI : fetchEvent(start, end, filter)
    fetchAPI --> InMemoryStorage : fetch by date and apply filter
    fetchAPI <-- InMemoryStorage : list of events
    Caller <-- fetchAPI : List<Events>, isTruncated


Requirements fetchEvent:
^^^^^^^^^^^^^^^^^^^^^^^^


Fetch Event:
''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **When** publish an arbitrary event that matches the fetchAPI filter
|  **And** calling the fetch API with an filter matching the event and time range
|  **Then** the result list contains the event

Fetch No Event On Filter Mismatch:
''''''''''''''''''''''''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **When** publish an arbitrary event that matches the fetchAPI filter
|  **And** calling the fetch API with an filter not matching the event
|  **Then** the result list is empty

Fetch No Event On Time Range Mismatch:
''''''''''''''''''''''''''''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **When** publish an arbitrary event that matches the fetchAPI filter
|  **And** calling the fetch API with a not matching time range but matching filter
|  **Then** the result list is empty

Backend Is Empty:
'''''''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **And** no events stored
|  **When** calling the fetch API with an arbitrary filter and/or time range
|  **Then** the result list is empty

Fetch-API not supported by Backend:
'''''''''''''''''''''''''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **And** the backend plugin has no fetchAPI implemented
|  **And** contains an arbitrary number of events
|  **When** calling the fetch API with valid parameters
|  **Then** the result list is empty
|  **And** no error is returned

Time Range Is Invalid:
''''''''''''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **And** contains an arbitrary number of events
|  **When** calling the fetch API with an invalid time range
|  **Then** an error is returned
|  **And** the result list is empty 

RPN-Filter Is Invalid:
''''''''''''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **And** contains an arbitrary number of events
|  **When** calling the fetch API with an invalid RPN-Filter
|  **Then** an error is returned
|  **And** the result list is empty 

An Internal Error Occures:
''''''''''''''''''''''''''

|  **Given** an elosd with an configured "fetchAPI" backend
|  **And** and contains an arbitrary number of events
|  **And** the log level of elosd is at least set to `Error`
|  **When** calling the fetch API with valid parameters
|  **And** arbitrary internal error not occures
|  **Then** the result list is empty 
|  **And** an error is logged internaly
