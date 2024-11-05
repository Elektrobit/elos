Elos Source
===========

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   clients/index
   common/index
   components/index
   demos/index
   elosd/index
   libelos/index
   libelos-cpp/index
   libelos_lite/index
   libelosdlt/index
   libelosplugin/index
   libelos_common/index
   plugins/index
   version/index


Intro
-----

The event logging and management system (elos) is intended to implement
and provide a unified system monitoring interface standard. The main
task is to collect various information about the system and convert them
into a canonical format and therefore annotate the information with a
specific set of meta data if possible. The thus obtained and refined
system information are then :

-  persistently stored in a canonical format
-  published in a canonical format

The spectrum of system information is wide and ranges from syslog and
kmsg to core dumps to measurements obtained from proc- and sys-fs like
for example temperatures. Those system information are meant to obtained
via so called Scanner which are basically Plugins as shared objects and
by any other application capable to use the provided socket.

Requirements
------------

For now the requirements are simplified into the following points:

-  Scan for different events and store them for later retrieval
-  Deliver list of selected events to one or more applications on
   request
-  Ability to push events to elos from an application
-  Ability to change events, access rights and other parameters via
   configuration files
-  Ability to easily extend elos with new events later in development
-  Ability to easily run in different environments
-  Ability to communicate with elos from different languages
   (e.g. Python)
-  Separate event scanner dependencies from elos itself to prevent
   dependency hell
-  Thread safe library functions
-  Support multiple user, multi processes
-  Be performant and efficient
-  …

The input and output data formats
---------------------------------

The input data processed by elos can divided into two types: \* Non
canonical or non normalized Input (Raw Input) \* Canonical or normalized
Input (Canonical Input)

The following diagram shows the flow of canonical and non canonical
inputs. |Data Flow Diagram|

The canonical event entity can then be published and persistently logged
which means stored on a non volatile storage for later evaluation.

The event logging and management system daemon has two types of
interfaces to obtain input:

-  Scanner-API
-  Client-API

The Client-API uses a communication protocol where events are encode in
JSON and therefore the canonical format is already enforced.

The Scanner-API also enforce the canonical format of an event from a
scanner implementation for an arbitrary source. The scanner
implementation itself must contain the functionality to convert input
data from an arbitrary source into the canonical format of an event.

Components
----------

Based on the requirements we can simplify Elos into the following
abstract components:

-  EventProcessor: Elos itself as the message broker which receives and
   distributes events based on previously defined event filters.
-  LogAggregator: Persist events that fits (SM_REQ001) and is therefore
   the implementation for the required DataCollectionFacility (DCF)
-  Scanner: Modular part (shared object) of the program that scans for
   events and publish them
-  Library: C library that contains everything to communicate with the
   server

Components using elos: - Client: Applications which subscribe to or
publish events by using the C-Library (or directly to the socket) -
Kernel: delivers log messages via its interface to the event logging and
management system daemon - Syslog-ing application: Application that uses
syslog() to write log infos (most FOSS does this)

Design decision - Basic principle of processing inputs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  Filtering of input data is done by the scanner in any case

A) Logging of events is done seperately by each event source
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Input - Is a arbitrary data point (String, number , …)

processing: - If matched by filter rule then: - If event should be
published pass it to the EventAssembler which creates and publish an
event containing the input data - If event should be logged pass it to
the LogAssebler which creates and stores the ``event``

Commitment:

-  Not each event is automatically logged
-  The EventProcessor receives, filters and dispatches (or drops)
   published events
-  Each Scanner contains the logic on how to filter and convert raw
   inputs into an canonical event
-  Access to LogAggregator must be synchronized for multiple scanner
   threads
-  Knowledge and Logic for new Log sources can be extended separately
   through shared library!

.. figure:: /doc/images/component_diagram_A.png
   :alt: Component diagram A

   Component diagram_A

B) Logging is done by the EventProcessor
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

input - Is a arbitrary data point (String, number , …) - Each input
``Data`` is an event if passes the filter criteria

processing: - Each event is published - Each published event gets logged
by the LogAggregator in the EventProcessor

Commitment:

-  Data is published as it is as event payload
-  Each event is logged through LogAggregator
-  Logging is done by EventProcessor
-  Scanner has no knowledge about Data format
-  Knowledge and Logic is possibly scattered through EventProcessor and
   LogAggregator

.. figure:: /doc/images/component_diagram_B.png
   :alt: Component diagram B

   Component diagram_B

Conclusion
^^^^^^^^^^

-  We use A as :

   -  Separate Log-Features from event handling (separation of concerns)
   -  Better performance in the EventProcessor because we do not bother
      with logging
   -  Knowledge and logic for new Log sources can be extended separately
      through shared library!

.. _elos-1:

elos
~~~~

Elosd has the following tasks:

-  Receive and dispatch events based on user defined event filters
   (EventProcessor)
-  Persist certain event messages on a non volatile storage
-  Maintain the scanner plugins (Scanner Manager)

The EventProcessor
^^^^^^^^^^^^^^^^^^

Elos is required to accept messages from scanner plugins or different
applications generated through arbitrary events and dispatch them to
corresponding event lists (message queues).

communication patterns
''''''''''''''''''''''

Requirements:

-  N to M communication (N message producer to M message consumer)
-  Message producer do not need to know anything about message consumers

Possible approaches using a message broker architecture

-  Central instance controlling publishing of message
-  House keeping, resource control at a central known controllable point

1) Publish & Poll

The message dispatching follows the “publish & poll” or the “message
queue / mailbox” communication pattern
(https://en.wikipedia.org/wiki/Message_queue)

-  Messages will be fetched by the client

2) Publish & subscribe

Others like the “publish & subscribe” communication pattern
(https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern) are in
discussion if required.

-  Message is directly pushed to the client, the client gets directly
   notified about new messages

Actual implementation
'''''''''''''''''''''

.. figure:: /doc/images/publish_poll.png
   :alt: Publish&Poll Component diagram

   Publish & Poll

In general there are the following roles:

-  EventProcessor:

   -  Receives published events
   -  Compares the received items with the previously configured
      EventFilters

      -  Append the event to the EventFilter’s EventQueue in case of a
         match
      -  Drop the event in case there were no matches

   -  Only one instance present (running multiple instances is not
      intended)

-  Subscriber:

   -  Is a client
   -  Registers for specific events based on EventFilterStrings
   -  The number of subscribers is limited by design only by system
      resources
   -  Subscribers connect via TCP/IP socket
   -  Has to maintain an active connection represented by a session to :

      -  Poll for events on a regular basis
      -  Keep created EventFilters and EventQueues alive

   -  Number of EventQueues a subscriber can create is limited by the
      type of the EventQueueID
   -  Currently EventFilterStrings are not shared or compared, so
      registering two times to receive events with (e.g.) messsage code
      8004 will result in two fully independent EvenQueues.

-  Publisher<client,scanner>:

   -  Send events to the EventProcessor for distribution
   -  Can publish an arbitrary amount of independent events per
      session/connection
   -  Multiple publishers can publish the same event

-  Client:

   -  Can be any arbitrary process who is capable to connect to the
      elosd socket. (this includes remote process on other machines)
   -  Can be both Subscriber and Publisher
   -  Number is limited at least by CONNECTION_MANAGER_MAX_CONNECTIONS +
      CONNECTION_MANAGER_LISTEN_QUEUE_LENGTH

-  Scanner:

   -  Are dynamic loadable shared objects to extend the basic
      functionality of elosd by publishing arbitrary information
      directly via the EventProcessor
   -  Not explicitly limited in number, but by system resources like
      open file descriptors or thread count

The relevant entities are related as follows:

.. figure:: /doc/images/event_class_diagram.png
   :alt: Event class diagram

   Entities

-  Event

   -  Is the entity which is published
   -  Corresponds usually to one publisher, but it is not excluded that
      multiple publishers publish the same events (i.e. two publishers
      observing the syslog will publish the same log line)
   -  Event lifetime depends on existing EventFilters and EventQueues,
      if the Event itself doesn’t match any existing EventFilters while
      published, it is dropped, otherwise it will reside in an
      EventQueue until it is read or overwritten by newer Events.

-  EventFilter

   -  Receives a unique EventFilterID during creation
   -  Exists only as long as the corresponding client has an active
      connection
   -  Matches Events based on the given EventFilterString (e.g. events
      with messageCode 8004)

-  EventQueue (message queue/box)

   -  Receives a unique EventQueueID during creation
   -  Exists only as long as the corresponding client has an active
      connection
   -  Is limited in capacity, which can be controlled through the API
   -  Drops oldest messages first on overflow

-  EventFilterNode

   -  Receives a unique EventFilterNodeID during creation
   -  Exists only as long as the corresponding client has an active
      connection
   -  Connects one or more EventFilters with one EventQueue
   -  Allows for much easier Event processing (“if any EventFilter in
      EventFilterNode matches, add Event to EventQueue”)

**Publishing**

.. figure:: /doc/images/publishing_sequence_diagram.png
   :alt: Publishing sequence diagram

   Publishing

**Subscribing**

.. figure:: /doc/images/subscribing_sequence_diagram.png
   :alt: Subscribing sequence diagram

   Subscribing

.. _actual-implementation-1:

Actual implementation
'''''''''''''''''''''

.. code:: json

   {
     "date":[1362,682302528],
     "source": {"appName":"","fileName":"","pid":0},
     "hardwareid":"INVALID",
     "payload":"arbitrary data",
     "classification": 0,
     "severity": 0,
   }

.. |Data Flow Diagram| image:: /doc/images/data_flow.png
