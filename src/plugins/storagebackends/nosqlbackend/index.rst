NoSql – Storage Backend
=======================


As an example for an NoSql database, MongoDB was chosen in order
to evaluate write performance of NoSql databases.

Currently, only a Proof-of-Concept implementation exists and shall be used with
caution.

Configuration
-------------

In order to use InfluxDB as a logging backend, the following configuration is
necessary:

.. code-block::

   ...
   "EventLogging": {
       "PluginSearchPath": "/usr/lib/elos/backend",
       "Plugins": {
           "NoSQLBackend": {
               "File": "backend_nosql.so",
               "Run": "always",
               "Filter": [
                   "1 1 EQ"
               ],
               "Config": {
                   "ConnectionString": "mongodb://mongoadmin:secret@elos-mongo:27017"
               }
           }
       }
   },
   ...

**ConnectionString**: define how to connect to mongoDB instance


Database Schema
---------------

The plugin creates a database `elosd` and stores all events in a table
`events`. The event structure is flattened so that child fields of `event.date`
and `event.source` becomes a member of the BSON object which is stored in the
event table.

+-------------+-----------------------+-------------------------+------------------+----------------+-----------------+------------------+-------------+----------------+-----------------+
| _id <OID>   | messageCode <INT32>   | classification <INT64>  | severity <INT32> | date_s <INT64> | date_ns <INT64> |  payload <UTF8>  | pid <INT64> | appName <UTF8> | fileName <UTF8> |
+=============+=======================+=========================+==================+================+=================+==================+=============+================+=================+
|  ...        |   2000                |    0x01                 |   0x01           |   1713187301   |       0         | "custom payload" |    42       |   "myApp"      | "/bin/myApp"    |
+-------------+-----------------------+-------------------------+------------------+----------------+-----------------+------------------+-------------+----------------+-----------------+


Testing mongoDB backend
-----------------------

To run elos with mongodb plugin, a `docker-mongodb-run.sh` script is provided
in the `./ci/` folder.

Configure elosd to mongoDB :

Copy the configuration snippet for `NoSQLBackend` from the example
configuration above into the Plugins section of `EventLogging` in
`./test/integration/service/config.json`.

To start a mongoDB use :

.. code-block::

   ./ci/docker-mongodb-run.sh

Then start elosd :

.. code-block::

   LINK_NOSQL=true ./ci/docker-target-run.sh

