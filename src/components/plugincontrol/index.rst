PluginControl
=============

The PluginControl component is responsible for handling a Plugin.
One PluginControl is responsible for one Plugin, which includes everything
from loading the shared object, verifying its type and its functions,
as well as starting and stopping it with the help of a worker thread.

Each Plugin needs a PluginConfig data structure with the name
of 'elosPluginConfig' present to set the Plugins type and the functions
that shall be used (which can be declared as static).

.. code-block::

    elosPluginConfig_t elosPluginConfig = {
        .type = PLUGIN_TYPE_STORAGEBACKEND,
        .load = _pluginLoad,
        .unload = _pluginUnload,
        .start = _pluginStart,
        .stop = _pluginStop,
    };

The available Plugin types are SCANNER, STORAGEBACKEND and CLIENTCONNECTION.
The plugin types are currently still in its infant stages and will be extended
in the near future, which will also result in additional documentation here.

Each Plugin can be in three states: Unintialized, Loaded and Started.
The Plugin is "Uninitialized" until elosPluginControlLoad is called, which will
open the shared object, verify its contents and creates a worker thread for
the Plugin that will be sleeping until elosPluginControlStart is called.
After elosPluginControlLoad was called the Plugin is in the "Loaded" state.
Once the Plugin is started, it will be running and in the "Started" state until
elosPluginControlStop is called (after it will be in the "Loaded" state again).
A stopped Plugin can be then unloaded with elosPluginControlUnload, putting it
back into the "Unintialized" state. Note that Starting and Stopping a Plugin
multiple times is currently not supported, but will be added at a later stage,
so a Plugin should make sure it is capable to do so.

.. uml::
   :caption: PluginControl and Plugin data flow

    elosd -> PluginControl : PluginControlInitialize
    elosd -> PluginControl : PluginControlLoad
    PluginControl -> Plugin : <open shared object>
    PluginControl -> Plugin : <verifying plugin>
    PluginControl -> Plugin : <creating worker thread>
    Plugin -> Plugin : <call function in PluginConfig.load\nin worker thread>
    Plugin -> Plugin : <worker thread sleeps,\n waiting for start command>

    elosd -> PluginControl : PluginControlStart
    PluginControl -> Plugin : <wake up worker thread>
    Plugin -> Plugin : <call function in PluginConfig.start\n in worker thread>
    Plugin -> Plugin: <PluginConfig.start function\nkeeps running in worker thread>

    elosd -> PluginControl : PluginControlStop
    PluginControl -> Plugin : <call function in PluginConfig.stop>
    Plugin -> Plugin : <PluginConfig.start function\nstops in worker thread>

    elosd -> PluginControl : PluginControlUnload
    PluginControl -> Plugin : <close/join worker thread>
    PluginControl -> Plugin : <call function in PluginConfig.unload>

    elosd -> PluginControl : PluginControlDeleteMembers

Notes:

-  The Plugin state is managed by PluginControl,
   the Plugin itself does have no direct knowledge of the state it is in.

-  All Plugin related data must be stored in the elosPlugin_t data structure,
   using globals will cause problems, as a Plugin can be loaded multiple times
   with different configurations.

-  Each PluginControl call that has a Plugin equivalent (e.g. Load, Unload,
   Start, Stop) will also result in a Plugin call of the function of the same
   name (.load, .unload, .start, .stop). Leaving functions out is not supported
   and will result in errors when it is loaded.
