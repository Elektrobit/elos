DLT – Storage Backend
=======================

The DLT-backend plugin is currently a PoC to evaluate how to forward elos
events to a Diagnostic Log and Trace infrastructures. It uses the
:file:`/src/libelosdlt/` client protocol implementation.

Configuration
-------------

The configuration of the DLT storage plugin follows the usual pattern. To add
an instance of a DLT storage backend add an entry like the following in the
`/root/elos/EventLogging/Plugins` section in your elos configuration.

.. literalinclude:: /src/components/config/elosd.json
   :language: json
   :lineno-start: 64
   :lines: 64-74

* **Connection**: the connection string used to connect to a DLT daemon (currently only a path to DLT-pipe is supported)
* **EcuId**: The ECU ID to be used in the log message.
* **AppId**: The Application ID to be  used in the log message.

You can find this example in :file:`/src/components/config/elosd.json`.

Build
-----

The plugin is build if `ELOSD_EVENTLOGGING_BACKEND_DLT` is defined in cmake.
