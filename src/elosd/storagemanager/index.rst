Storage Manager
===============

To store historical events, elos is able to use multiple storage backends.
These backends are provided as elos-plugins. The Storage Manager is responsible
for loading all plugins via the generic Plugin Manager
:doc:`/src/elosd/pluginmanager/index`, starting, stopping and unloading them.
The Storage Manager also determine the configuration which is used to search
for plugins.


Static structure
----------------

The Storage Manager is a elosd component (:doc:`/src/elosd/index`).
It provides as subcomponents the

* PluginFilterLoader
* StorageBackend.


PluginFilterLoader
~~~~~~~~~~~~~~~~~~

Storage Backends can define a list of event filters to specify which events are
accepted and processed by the corresponding Storage Backend. These filters are
located in the plugin configuration for a Storage Backend. The
PluginFilterLoader is responsible to locate those event filter configuration
and initialize all event filters for each Storage Backend.

The PluginFilterLoader need an initialized PluginControl object to operate on.
The initialized event filters are then stored and associated within the
corresponding Storage Backend. A missing or an empty filter list will result in
an loaded storage backend that can never receive events, as no filters can
report a match.

The configuration object of an plugin shall contain an array of strings for the
key `Filter` of strings.

.. code-block:: json 
  :caption: structure of the expected configuration

  {
    "Filter": [
      ".event.messageCode 1000 GT"
    ]
  }

The operation has two possible results, either

* SAFU_RESULT_OK on success and a list of all filters or an empty filter list
  is created

or

* SAFU_RESULT_FAILED if something gone wrong during the filter setup.
  Corresponding error logs shall indicate the root of the problem.


StorageBackend
~~~~~~~~~~~~~~

The Storage Backend provides functionality which is common for all storage plugins.

It provides the function `elosStorageBackendAccepts` to check whether an event
shall be processed by a plugin or not. This functionality is currently used by
the LogAggregator.

To check if a Storage Backend shall persist an event, all filters are applied
to the event in order of their appearance in the event filter vector.

The operation has three possible results :

1. First successful match of an event filter will return SAFU_RESULT_OK
2. None of the event filters match will return SAFU_RESULT_NOT_FOUND
3. An error occurs while iterating and executing the event filters and
   SAFU_RESULT_FAILED is returned.


Dynamic behavior
----------------

See below the lifecycle and control flow of the Storage Manager.
        
.. uml::
   :caption: The lifecycle of the Storage Manager

   == Initialization ==
   -> StorageManager: elosStorageManagerInitialize()
   StorageManager -[#red]> ElosConfig: fetch EventLogging plugin configurations
   
   == Start ==
   -> StorageManager: elosStorageManagerStart()
   StorageManager -[#red]> PluginManager: elosPluginManagerLoad()
   StorageManager <[#blue]-- PluginManager: Return plugin loaded and started
   StorageManager -[#red]> Plugin: load and set eventilter
   
   == Stop ==
   -> StorageManager: elosStorageManagerStop()
   StorageManager -[#red]> Plugin: cleanup eventilter
   StorageManager -[#red]> PluginManager: elosPluginManagerUnload()
   StorageManager <[#blue]-- PluginManager: Returns result
   
   == Cleanup ==
   -> StorageManager: elosStorageManagerDeleteMembers()
   StorageManager -[#red]> StorageManager: free resources

Data
----

The storage manager expects a configuration object under the path
`/root/elos/EventLogging`. The object must follow the specification required by
the PluginManager. Each Plugin needs a filter option to be able to receive
events. Have a look on the PluginFilterLoader and StorageBackend for details.

.. code-block:: json

   {
       "root": {
           "elos": {
               "EventLogging": {
                   "PluginSearchPath": "/usr/lib/elos/backend",
                   "Plugins": {
                       "a Plugin name": {
                           "Filter": [".event.severity 1 GT",".e.messageCode 42 EQ"]
                       }
                   }
               }
           }
       }
   }

Error
-----

If the configuration for Storage Plugins is not found or invalid the
initialization of the StorageManager will fail. The StorageManager will fail to
start if the PluginManager reports error on loading plugins. But will continue
to start if some backends have no or broken filter rules. In that case errors
are reported but elos continues operation. Probably without being able to
persist any events.
On errors during stopping and freeing memory the StorageManager try to cleanup
as far as possible. An StorageManager instance that has failed to stop or to
delete its members shall not be restarted or reinitialized.
