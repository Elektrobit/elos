JSON-File – Storage Backend
===========================

This is changeable by either configuring the option “StoragePath” unter
“Config”, or by defining ``ELOS_STORAGE_BACKEND_<PLUGIN NAME>_FILE`` in
the enviroment. If defined the enviroment variable takes precedent over
the file configuration. The definition of “StoragePath” is mandatory,
but it can be defined as empty, in which case the default value,
``/var/log/elosd.log`` will be used.

Another, in its case optional, configuration option is “Flags”, which
allows to define additional flags with which the log file is opened. If
specified, its value is a json list of strings contaning all options
that are additionally wanted. Currently the following list of flags are
possible to be used: - ``O_NOATIME`` - ``O_DIRECT`` - ``O_SYNC`` -
``O_DSYNC``

As long as the value is a valid json list, any unspecified or non-string
option will simply be ignored with a warning, instead of creating an
error and exitting.

An example configuration of two coexisting json backend loggers using
all options could look as following:

.. code:: json

   "Plugins": {
       "RegularLog": {
           "File": "backend_json.so",
           "Run": "Always",
           "Filter": [ ".messageCode 5005 NE" ],
           "Config": {
               "StoragePath": "/tmp/elosd_%host%_%date%_%count%.log",
               "Flags": ["O_DSYNC", "O_NOATIME"],
               "DateFormat": "%m-%d-%Y",
               "PathSizeLimit": 1000
               "MaxSize": 100000
           }
       },
       "CoreDump": {
           "File": "backend_json.so",
           "Run": "Always",
           "Filter": [ ".messageCode 5005 EQ" ],
           "Config": {
               "StoragePath": "/tmp/coredump.log",
               "Flags": ["O_SYNC", "O_DIRECT"]
           }
       }
   }

This configures two loggers, one logging all non-coredump events to
/tmp/elosd.log, which is opened using the ``O_DSYNC`` and ``O_NOATIME``
options. The second one logs all coredump events to /tmp/coredump.log
using the ``O_SYNC`` and ``O_DIRECT`` options.

The first logger additionally uses variables for its logs name, as well
as log rotation. The ``%*%`` macros get replaced by following values: -
host: the host machines name as registered by safu - date: the date of
creation of the file - count: a counter numbering files with, besides
the counter, exactly the same filename.

The DateFormat field, which the first logger has defined is an optional
field that defines the format of the date which replaces ``%date%``.
This formatting uses the strftime function of time.h and the string and
its conversion follows strftimes conversion specifications. If
DateFormat is not given, the used format string is ``%Y%m%d``.

If ``%date%`` is defined, date-based log rotation gets enabled.The log
files gets rotated whenever the formatted date string for the file
changes. This allows to configure a rotation every second, but also
every year, depending on the dates format string.

The MaxSize field defines an upper limit for a json loggers log file
size. When the file reaches the size limit, and ``%count%`` is defined,
the logfile will rotate. It is necessary to make this rotation dependend
on ``%count%`` to ensure the uniqueness of the new filename after the
rotation. If MaxSize is not set, it will default to a maximum size of
10000 bytes. This default value can be overwritten by setting the define
``ELOS_JSON_LOGGER_MAX_FILE_SIZE`` during compile time.

The PathSizeLimit field is another optional field, which limits the size
of the, macro resolved, file path. If the final file path exceeds the
limit, the logging file path will default to ``/var/log/elosd.log``.
This can be changed by defining ``STORAGE_LOCATION`` at compile time.

If PathSizeLimit itself is not set, it will default to 1024. This value
can be changed by defining ``ELOS_JSON_LOGGING_PATH_LIMIT_DEFAULT`` at
compile time.


