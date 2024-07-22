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

#. The PluginManager is initialized with call to elosPluginManagerInitialize, where PluginControlVector and PluginManager structs are initialized and its state is set to `PLUGINMANAGER_STATE_INITIALIZED`.
#. Then the Module (Scanner, Client, Storage Backend) Managers call the `elosPluginManagerLoad` and passes the Module Config, so that the plugins in the config can be loaded into the PluginControlVector
#. When the PluginManager receives the module config, the configuration is checked for well formed plugin configurations.
#. If checks fail due to ill formed plugin configuration:
    * The corresponding module plugins are not loaded and the Plugin Manager returns a `SAFU_RESULT_FAILED` to the module
    * When the Plugin Manager fails the load the plugins, the Module also fails.
    * When the Module Fails then the elos process is terminated
#. When no ill formed configs are found, then the check succeeds and the plugins are loaded normally.


Required Functionality
----------------------
#. When plugin manager encounters an ill formed plugin configuration in the module, then it shall not load the plugin but continues loading other plugins. The failure to load the plugin is intimated via a warning, but it does not terminate the process. This needs to be implemented.

    .. code-block:: json
       :caption: Module plugin configuration with ill formed ``File`` and ``Run``.
       :emphasize-lines: 5-6
    
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

#. When a module configuration has no plugins i.e. `Plugins` have 0 plugins defined, the expectation being plugins may be defined in the future, then the module is loaded with out plugins and process does not get terminated. This requirement is already met in the current code.

    .. code-block:: json
       :caption: Module plugin with no plugin configuration.
       :emphasize-lines: 3
    
                "Module": {
                    "PluginSearchPath": "/usr/local/lib/elos/module",
                    "Plugins": {}
                }

#. When a configured plugin fails to load or start then the process shall terminate, because it might cause an improper clean up resulting in memory leak. This requirement is already met in current code.

    .. code-block:: json
       :caption: ModulePluginOne fails to load.
       :emphasize-lines: 4
    
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

#. When a plugin shared object file in the configuration can not be found, then the process shall terminate, this is because with out the shared object the required functionalities are not available for the plugin. This requirement is already met in current code.

    .. code-block:: json
       :caption: ModulePluginOne shared object file not found.
       :emphasize-lines: 5
    
                "Module": {
                    "PluginSearchPath": "/usr/local/lib/elos/module",
                    "Plugins": {
                        "ModulePluginOne": {
                            "File": "module_plugin_one.to",
                            "Run": "always"
                        },
                        "ModulePluginTwo": {
                            "File": "module_plugin_two.so",
                            "Run": "always"
                        }
                    }
                }

#. When a plugin shared object file path in the configuration is not the correct path or invalid, then the process shall terminate. This is because with out the correct path the plugin shared object can not be loaded properly. In the current code when the path to plugin shared object is invalid, the plugin is loaded. This should not be the cases. This needs to be implemented.

    .. code-block:: json
       :caption: ModulePluginOne shared object file path is invalid.
       :emphasize-lines: 2
    
                "Module": {
                    "PluginSearchPath": " ",
                    "Plugins": {
                        "ModulePluginOne": {
                            "File": "module_plugin_one.to",
                            "Run": "always"
                        },
                        "ModulePluginTwo": {
                            "File": "module_plugin_two.so",
                            "Run": "always"
                        }
                    }
                }

#. When loading the plugin shared object file via `ld_open` failed, the process shall terminate. This is implemented in the current code.
