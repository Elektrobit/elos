libelosplugin
=============


Intro
-----

The libelosplugin contains everything needed for creating a new elos plugin.
Each plugin can be one of the three following types mentioned below.

Scanner
~~~~~~~

The scanners main purpose is to watch certain system resources (e.g. free space
on hard drives, temperatures, e.t.c.) and generate Events if certain
conditions are met.

StorageBackend
~~~~~~~~~~~~~~

The StorageBackends acts as an interface between a form of persistent storage
(e.g. a json file, sql database, e.t.c.) and elos itself. It takes Events
and writes them into the storage solution it is based upon.

ClientConnection
~~~~~~~~~~~~~~~~
The ClientConnection acts as an interface between elos and everything that
wants Events from elos, e.g. by subscribing to certain Events with an
EventFilter selector.


Configuration
-------------

To create a plugin a shared object needs to be created that links to
libelosplugin, it also needs a configuration struct present in one of
the source files that defines the type of the plugin as well as the
functions to use. This struct is of the type elosPluginConfig_t and should
be defined at the bottom of the file since it makes referencing functions
easier. The name of the struct must always be "elosPluginConfig" to make
it possible for the elosPluginControl to find the structure. The functions
can (and perhaps should) be marked as static, as symbol table lookups are
slightly faster with fewer entries present.

Example::

    elosPluginConfig_t const elosPluginConfig = {
        .type = PLUGIN_TYPE_SCANNER,
        .load = _loadFunction,
        .unload = _unloadFunction,
        .start = _startFunction,
        .stop = _stopFunction,
    };

The Load functions allocates memory and initializes the plugin, while Unload
does the opposite and frees the data associated with it. The Start and Stop
functions are responsible for fully activating the Plugin, with Start for
example containing an endless loop that monitors a resource, while Stop
contains the functionality to end the loop running in Start.

All used functions are of the type :code:`safuResultE_t func(elosPluginContext_t *)`,
with the elosPluginContext_t containing everything needed to operate the plugin.


Connection Manager Framework
----------------------------

Because elosd client plugins can provide various kinds of connections (TCP,
Unix domain sockets, UDP, vsocks, or any other file-descriptor-based
connections), they must follow a common procedure:

* read and provide client plugin configuration
* setup listening interface
* accept connections and create a client session (connection pooling)
* authorize a client
* dispatch the client session to a worker thread and connect it to elosd core
* receive and send elos protocol messages
* filter publishing attempts

Although the specifics differ by connection type, most of the underlying logic
remains consistent. To minimize code complexity and redundancy, the Connection
Manager Framework serves as an abstract component that encapsulates shared,
generic tasks.

It consist out of three main components witch requires some functions to be
implemented by a client plugin implementation. See the following component
diagram to get an idea how the Framework components and the client specific
implementations parts are related.

.. image:: /doc/images/connectionmanager_component.png
   :alt: Overview relation between client plugins and Connection Manager Framework


Connection Manager
~~~~~~~~~~~~~~~~~~

It implements on a high level the logic start, stop the socket listening
thread, to maintain the connection pool and authorize incoming client
connections. As these task differ for different kinds of sockets, they have
to be implemented as callbacks from the client plugin (referred to as client
in the text below).

* `safuResultE_t (*elosConnectionManagerInitializeListener_t)(elosConnectionManager_t *connectionManager, elosPlugin_t const *plugin);`

    This function is called by the Connection Manager to setup the listen
    interface. The Client implementation shall create and initialize the
    socket as needed.

    The client can use `connectionManager->connectionManagerContext`
    create a custom context to store and pass around connection type
    specific states as a custom data type.

    The client must have set the `connectionManager->fd` to the listen
    socket, as it used by the Connection Manager.
      
* `safuResultE_t (*elosConnectionManagerListenerAcceptConnection_t)(elosConnectionManager_t *connectionManager, elosClientConnection_t *connection);`
  
    This function is called from the Connection Manager when a incoming
    connection is registered. The Connection Manager provides the associated
    Connection from the Connection Pool.

    The client have to make the appropriate `accept` syscall on the
    `connectionManager->fd`.

    The client can implement here any additional session handling as needed.

* `safuResultE_t (*elosConnectionManagerCloseListener_t)(elosConnectionManager_t *connectionManager);`

    This function is called from the Connection Manager when the listener
    thread is stopped.

    The client have to shutdown and close the socket appropriate to it's type.
    Further cleanup task and freeing of resource can be done in
    `elosConnectionManagerDeleteListener_t`.

* `safuResultE_t (*elosConnectionManagerDeleteListener_t)(elosConnectionManager_t *connectionManager);`

    This function is called from the Connection Manager when the listener gets
    finally deleted and shall never be used.

    The client have to delete any allocated resources related to
    `connectionManager->connectionManagerContext` at this point.

* `safuResultE_t (*elosConnectionManagerSetClientConnectionHandlers_t)(elosClientConnection_t *connection);`

    This function is called from the Connection Manager during setup of the
    Client Connection pool. 

    The client have to set all the required function pointers in
    `elosClientConnection_t`.

* `int (*elosGetConnectionLimit_t)(elosPlugin_t const *plugin);`

    This is called by the Connection Manager during initialization phase to
    determine the size of the connection pool. Note, that the maximum size for
    the connection pool is limited to `ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT`
    (default 200).

    
Client Connection 
~~~~~~~~~~~~~~~~~

* `safuResultE_t (*elosClientConnectionInitializeConnection_t)(elosClientConnection_t *connection);`

  This function is called during the initialisation of the Connection Pool by the Connection Manager.

  The client shall initialize and allocate necessary resource needed by
  `elosConnectionManagerListenerAcceptConnection_t` to establish a new client
  connection.

  The client can use `connection->clientConnectionContext` to attach any type
  of data to a connection. Which can be used later i.e. the actual `socketaddr`
  struct type.
    
* `safuResultE_t (*elosClientConnectionCloseConnection_t)(elosClientConnection_t *connection);`

   This is called by the Client Connection worker thread when a connection needs
   to be closed for any reason.

   The client shall implement an appropriate socket shutdown and close
   procedure for the corresponding socket type. The Client Connection object
   will be returned to the Connection Pool, thus the object must be in a state
   to be used for new connections.

* `safuResultE_t (*elosClientConnectionDeleteConnection_t)(elosClientConnection_t *connection);`

   This method is called when the Connection Manager destroys the Connection Pool.

   The client shall free all allocate and associated resources. This object
   will not be reused anymore.


Client Authorization
~~~~~~~~~~~~~~~~~~~~

* `safuResultE_t (*elosClientAuthorizationInitialize_t)(elosClientAuthorization_t *const clientAuth);`

    This function is called when the Connection Manager is read to initialize
    the client authorization feature. After this call the
    `authorizedProcessFilters` of `elosClientAuthorization` get populated, so no
    need to do this by the client itself.

    The client has to setup any specific task and resource necessary to
    authorize an incoming connection.

    The client can store any context data in `clientAuth->socketData`.

* `safuResultE_t (*elosClientAuthorizationDelete_t)(elosClientAuthorization_t *const clientAuth);`

    This function is called by the Connection Manager when the authorization
    module has to be shutdown and free all allocated resources.

* `safuResultE_t (*elosClientAuthorizationIsValid_t)(elosConnectionManager_t *connectionManager, elosClientConnection_t *connection);`

    This function is called after the Client Manager accepts a new connection
    from the clients implemented `accept` method.

    The client shall check if the peer of the connection is a trustworthy
    process using the provided process filters in
    `clientAuth->authorizedProcessFilters`.



The following class and sequence diagram shall help to get a better overview
how components are related and how the Client Manager works and the control flow
actually is.

.. image:: /doc/images/connectionmanager_class.png

.. image:: /doc/images/connectionmanager_sequenz.png
   :alt: The control flow of a TCP Connection Manager in a Tcp-Client-Plugin.


