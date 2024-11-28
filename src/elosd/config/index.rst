Elos Configuration
==================

Configuring elos can be done in several ways thanks to the samconf-project.

Currently elos supports :

* Files (JSON)
* Environment variables
* Commandline arguments


The default config file is expected in `/etc/elos/elosd.json` and can be
overwritten by setting the environment variable `ELOS_CONFIG_PATH`.

Environment variables can only be used to overwrite option from configuration
files if `UseEnv` is set to true in  the elos main configuration file
`/etc/elos/elosd.json`.

If `UseEnv` is enabled the following configurations are equivalent.

::

    {
      "root": {
        "elos" {
          "LogLevel": "Warning"
        }
      }
    }

::

  ELOS_LOGLEVEL="Warning"

Elos use internally a config tree and do lookups for options using
a path. For example, to determine the configured log level it will use
`/root/elos/LogLevel`. For the Json file the path is quite obvious. For
environment variables the name has to include the full path and `_` is used as
path separator. The `/root` can be omitted on environment variables. It is a
samconf internal thing and will probably removed.


The configuration contains some elos global settings and then splits up in
different section.

Global Elos Options
-------------------

-  **UseEnv**: Define if elosd will allow overwrite configuration values
   by environment variables
-  **LogFilter**: Only log messages from these C-Files are shown, don’t
   touch it except you know what you’re doing (``ELOS_LOG_FILTER``
   default value: ``""``); files are separated by ``;``
   i.e. ``"first.c;second.c"``
-  **LogLevel**: Severity levels deciding how much information will be
   printed from no messages to extremely detailed the levels are: Off,
   Fatal, Error, Warn, Info, Debug and Verbose (``ELOS_LOG_LEVEL``
   default value: ``"Debug"``)
-  **ClientInputs**: This section defines plugins wich provides interface to
   allow other applications to communicate with elos. See
   :ref:`src/components/clientmanager/index:Client Manager` for more details on
   how to configure client plugins.
-  **EventLogging**: This section defines plugins wich are used by elos to
   store events. They are often also referred to as storage backend plugins. See
   :ref:`src/components/eventlogging/index:Event Logging` for more details on
   how to configure storage plugins.
-  **Scanner**: This section defines plugins wich are used by elos to observe
   and poll system sources to produce events. See
   :ref:`src/components/scannermanager/index:Scanner Manager` for more details
   on how to configure Scanner plugins.

Default Config
--------------

The default config is a good starting point to configure elos to your needs.

.. literalinclude:: /src/components/config/elosd.json
   :language: json
   :caption: elos default config shipped with elos
   :linenos:
