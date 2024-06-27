Plugin Manager
==============

The PluginManager initializes a :doc:`PluginControl <../plugincontrol/index>` for every plugin in the configuration provided to it.

Configuration
-------------

It can load plugins mutliple times for different plugin types. Each time expecting a configuration containing a map :code:`"Plugins"` that holds the configurations for all the plugins to load into a :code:`PluginControl`, a path to where the plugins can be found and a type want kind of plugin gets configured.

.. code-block:: json

   {
      "Plugins": {
         "a Plugin name": {
            "File": "plugin_file.so",
            "Run": "always",
            "Config": {
               "Port": 54321,
               "Interface": "127.0.0.1",
               "EventBlacklist": ".event.messageCode 2000 EQ",
               "authorizedProcesses": [
                  ".process.pid 1 EQ"
               ]
            }
         }
      }
   }

When no :code:`"Plugins"` list is provided it runs as if an empty list was given.


Dynamic behavior
----------------

#. The PluginManager is initialized with call to elosPluginManagerInitialize.
#. Then elosPluginManagerLoad is then called by the Module (Scanner, Client, Storage Backend) Managers
#. The PluginManager checks if the Manager Module Configuration have well formed Plugin Configurations
    * when ill formed configs are found :
        * then the corresponding plugins are not loaded
        * then the Plugin Manager returns a SAFU_RESULT_FAILED to the module
        * the SAFU_RESULT_FAILED result in tern terminates the process in main.c 
    * when well formed configs are found, the are loaded normally as intended.


Required Functionality
----------------------
#. When plugin manager encounters an ill formed plugin configuration in the module, then it shall not load the plugin but continues loading other plugins. The failure to load the plugin is intimated via a warning, but it does not terminate the process. This needs to be implemented.

    .. code-block:: json
    
                "Module": {
                    "PluginSearchPath": "/usr/local/lib/elos/module",
                    "Plugins": {
                        "ModulePluginOne": {
                            "Fle": "module_plugin_one.so",
                            "Run": "always"
                        },
                        "ModulePluginTwo": {
                            "File": "module_plugin_two.so",
                            "Ru": "always"
                        }
                    }
                }

#. When a module Configuration no plugins i.e. `Plugins` have 0 plugins defined, the expectation being plugins may be defined in the future, then the module is loaded with out plugins and process does not get terminated. This requirement is already met in the current code.

    .. code-block:: json
    
                "Module": {
                    "PluginSearchPath": "/usr/local/lib/elos/module",
                    "Plugins": {}
                }

#. When a configured plugin fails to load or start then the process shall terminate, because it might cause an improper clean up resulting in memory leak. This requirement is already met in current code.

    .. code-block:: json
    
                "Module": {
                    "PluginSearchPath": "/usr/local/lib/elos/module",
                    "Plugins": {
                        "ModulePluginOne": {
                            "File": "module_plugin_one.so",
                            "Run": "always"
                        },
                        "ModulePluginTwo": {
                            "File": "module_plugin_two.so",
                            "Run": "always"
                        }
                    }
                }
