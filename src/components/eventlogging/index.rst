Event Logging
~~~~~~~~~~~~~

The event logging system implements a data collection facility, which
collects events from different sources to filters and limits them in
order to store them. For details about the canonical event format see
“canonical event format”.

The Event Logging system has the following main goals:

-  filter events depending on filter settings
-  limit events to log if a configured limit (logs per time) is exceeded
-  optimize storage access to minimize IO operations and maximize
   physical storage lifetime

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

|overview event logging| #### Log aggregation

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
^^^^^^^^^^^^^^^^^^

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
'''''''''''''''''''''''''''''''''

A) JSON-File based

B) DBMS - SQLITE

C) Time series DB

D) NoSQL

E) Custom implementation

Storage backend Interface - Detailed Design (API)
'''''''''''''''''''''''''''''''''''''''''''''''''

The StorageBackend interface is defined as follows:

.. code:: c

   struct elosStorageBackend;
   typedef struct elosStorageBackend elosStorageBackend_t;

   typedef safuResultE_t elosStorageBackendStart_t(elosStorageBackend_t *backend);
   typedef safuResultE_t elosStorageBackendPersist_t(elosStorageBackend_t *backend, const elosEvent_t *event);
   typedef safuResultE_t elosStorageBackendShutdown_t(elosStorageBackend_t *backend);

   struct elosStorageBackend {
       const char *name;
       void *backendData;
       elosStorageBackendStart_t *start;
       elosStorageBackendPersist_t *persist;
       elosStorageBackendShutdown_t *shutdown;
   };

elosStorageBackendStart
                       

Initialize the event logging system. It initializes the configured
storage backend and setup synchronization and filter functionality.

Function:

.. code:: c

   typedef safuResultE_t elosStorageBackendStart_t(
       elosStorageBackend_t *backend
       );

Parameters: \* ``[in]`` backend -> a pointer to the backend instance

Returns: \* SAFU_RESULT_OK -> on success \* SAFU_RESULT_FAILED -> on
failure

elosStorageBackendShutdown
                          

Shutdown the logging system and free all related resources.

Function:

.. code:: c

   typedef safuResultE_t elosStorageBackendShutdown_t(
       elosStorageBackend_t *backend
       );

Parameters: \* ``[in]`` -> a pointer to a backend instance

Returns: \* SAFU_RESULT_OK -> on success \* SAFU_RESULT_FAILED -> on
failure

elosStorageBackendPersist
                         

Writes the event to the underlying storage.

Function:

.. code:: c

   typedef safuResultE_t elosStorageBackendPersist_t(
       elosStorageBackend_t *backend,
       const elosEvent_t *event
       );

Parameters: \* ``[in]`` backend -> a pointer to a backend instance \*
``[in]`` event -> a pointer to a event instance to persist

Returns: \* SAFU_RESULT_OK -> on success \* SAFU_RESULT_FAILED -> on
failure

Json Backend - Detailed Design (API)
''''''''''''''''''''''''''''''''''''

The JsonBackend interface is defined as follows:

.. code:: c

   safuResultE_t elosJsonBackendStart(elosStorageBackend_t *backend);
   safuResultE_t elosJsonBackendPersist(elosStorageBackend_t *backend, const elosEvent_t *event);
   safuResultE_t elosJsonBackendFindEvents(elosStorageBackend_t *backend, elosRpnFilter_t *filter, safuVec_t *events);
   safuResultE_t elosJsonBackendShutdown(elosStorageBackend_t *backend);

   typedef struct elosJsonBackend {
       int storageFile;
   } elosJsonBackend_t;

elosJsonBackendStart
                    

Initialize the file backed storage of JSON serialized event objects. The
``elosStorageBackend_t.backendData`` shall be initialized with a pointer
to a ``elosJsonBackend_t`` instance.
``elosJsonBackend_t.storageFileWrite`` and
``elosJsonBackend_t.storageFileRead`` shall contain valid file
descriptors to the target file for writing/reading purposes. In write
mode, the file shall be opened for appending and synchronous writing, to
ensure a write is carried out through the kernel caches.

Function:

.. code:: c

   safuResultE_t elosJsonBackendStart(
       elosStorageBackend_t *backend
       );

Parameters: \* ``[in]`` backend -> a pointer to the backend instance

Returns: \* SAFU_RESULT_OK -> on success \* SAFU_RESULT_FAILED -> on
failure

elosJsonBackendShutdown
                       

Shutdown the logging system and free all related resources.

Function:

.. code:: c

   safuResultE_t elosJsonBackendShutdown(
       elosStorageBackend_t *backend
       );

Parameters: \* ``[in]`` -> a pointer to a backend instance

Returns: \* SAFU_RESULT_OK -> on success \* SAFU_RESULT_FAILED -> on
failure

elosJsonBackendPersist
                      

Writes the event to the underlying storage pointed to by
``elosStorageBackend_t.backendData``. The function shall not call
``sync`` to flush kernel caches, as the file shall be opened with
O_SYNC. The function shall block until the event is successful written
to the storage backend.

Function:

.. code:: c

   safuResultE_t elosJsonBackendPersist(
       elosStorageBackend_t *backend,
       const elosEvent_t *event
       );

Parameters: \* ``[in]`` backend -> a pointer to a backend instance \*
``[in]`` event -> a pointer to a event instance to persist

Returns: \* SAFU_RESULT_OK -> on success \* SAFU_RESULT_FAILED -> on
failure

elosJsonBackendFindEvents
                         

Applies a filter to a given storage backend. Events which weren’t sorted
out by the filter, will be appended on the event vector.

Function:

.. code:: c

   safuResultE_t elosJsonBackendFindEvents(
       elosStorageBackend_t *backend,
       elosRpnFilter_t *filter,
       safuVec_t *events
       );

Parameters: \* ``[in]`` backend -> a pointer to a backend instance \*
``[in]`` filter -> a filter that will be applied to the backend \*
``[in|out]`` events -> an event vector, where elements will be appended

Returns: \* SAFU_RESULT_OK -> on success \* SAFU_RESULT_FAILED -> on
failure

.. |overview event logging| image:: /doc/images/overview_event_logging.png
