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

All used functions are of the type "safuResultE_t func(elosPluginContext_t *)",
with the elosPluginContext_t containing everything needed to operate the plugin.
