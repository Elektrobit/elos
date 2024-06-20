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

When no :code:`"Plugins"` list is providet it runs as if an empty list was given.

