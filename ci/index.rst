
.. default-role:: code

=================================
Different options on how to build
=================================

- `ci/build.sh [Debug|Release]`
   - uses `FetchContent`_ to install `cmocka_extensions`, `cmocka_mocks`, `safu` & `samconf`.
   - if `cmocka_extensions`, `cmocka_mocks`, `safu` & `samconf` are already installed in a path found by cmake that installation is used.
- `cmake && make|ninja`
   - can be configured to use `FetchContent`_ with variables but generally depends on them being installed already.

..  _FetchContent: https://cmake.org/cmake/help/latest/module/FetchContent.html

Build Configurations
====================

`cmake & make` without any options (default)
--------------------------------------------

.. code-block::

   cmake -B build/ .
   make -C build/

Builds everything except for the NoSQL and InfluxDB backend.
Everything is build in Debug mode.

.. code-block::

   make -C build/ install

Installs everything except `mock_libelos`.

Can be configured with the options described in section :ref:`cmake-options`.

`ci/install_deps.py`
--------------------

Intended to easier install the dependencies `cmocka_extensions`, `cmocka_mocks`, `safu` & `samconf`

.. code-block::

   ci/install_deps.py

By default it installs from the main branch of the GitHub repositories.
The behavior can be modified by specifying one of the following files in this order:

* file specified by `ELOS_DEPENDENCY_CONFIG` environment variable

.. code-block::

   export ELOS_DEPENDENCY_CONFIG=".myconfig/my_dependencies.json"

* `dependencies.json` in the root of the project

* `ci/dependencies_default.json` in the root of the project

Each resource can be defined by the `[url|path][commit|branch|tag][cmake_opts]`
options. This could look like:

.. code-block:: json

   {
      "safu": {
         "path": "../safu"
      },
      "samconf": {
         "url": "https://github.com/Elektrobit/samconf.git",
         "tag": "samconf-0.49.3"
      },
      "cmocka_mocks": {
         "url": "https://github.com/Elektrobit/cmocka_mocks.git",
         "commit": "2206cfbcf315b3c7381f9b9f4aa29238b18c1a6b",
         "cmake_opts": ["-D", "UNIT_TESTS=off"]
      },
      "cmocka_extensions": {
         "branch": "main"
      }
   }

If no URL or path is provided the default is used.
A path is used as is without checking the branch, tag or commit and includes
local changes that aren't committed jet.

The `url` property supports the evaluation of environment variables like
`SOURCES_URI` so it is possible to define template configurations like:

.. code-block:: json

   {
       "safu": {
           "url":  "${SOURCES_URI}/safu.git",
           "branch": "integration"
       },
       "samconf": {
           "url": "${SOURCES_URI}/samconf.git",
           "branch": "integration"
       },
       "cmocka_mocks": {
           "url": "${SOURCES_URI}/cmocka_mocks.git",
           "branch": "integration"
       },
       "cmocka_extensions": {
           "url": "${SOURCES_URI}/cmocka_extensions.git",
           "branch": "integration"
       }
   }

and call `install_deps.py` like :

.. code-block:: bash

   SOURCES_URI=https://${GIT_USER_TOKEN}@github.com/Elektrobit/

These dependencies get installed into `build/deps/` and if provided with an URL the sources can be found in `build/deps/src`.

In addition the `install_deps.py` provides the following command line options to tune the install process:

.. program-output:: ./ci/install_deps.py -h

To use binaries installed from dependencies add `build/deps/bin` to the `PATH` environment variable.

`ci/build.sh`
-------------

Intended for easier building of different configurations while developing and in CI.

.. code-block::

   ci/build.sh [BUILD_TARGET]

Builds everything except for the NoSQL and InfluxDB backend.
By default the BUILD_TARGET is Debug.
The build folder is set to `build/BUILD_TARGET/cmake` and everything is also directly installed into `build/BUILD_TARGET/dist/` (except for `mock_libelos`).

Additional configurations are described in :ref:`buildsh-options`.

Output
======

.. code-block::

   /usr
   ├── etc/
   │  └── elos/
   │     └── elos_log4c_demo/
   │        └── log4crc
   └── local/
      ├── bin/
      │  ├── demo_eloslog
      │  ├── demo_eventbuffer
      │  ├── demo_libelos_v2
      │  ├── demo_scanner_shmem
      │  ├── elos-coredump
      │  ├── elos_log4c_demo
      │  ├── elosc
      │  ├── elosd
      │  ├── elosMon
      │  ├── syslog_example
      │  └── tinyElosc
      ├── include/
      │  └── elos/
      │     ├── common/
      │     │  ├── message.h
      │     │  ├── process.h
      │     │  └── types.h
      │     ├── event/
      │     │  ├── event.h
      │     │  ├── event_source.h
      │     │  └── event_vector.h
      │     ├── libelos/
      │     │  ├── libelos.h
      │     │  └── libeloslog.h
      │     └── libelosplugin/
      │        └── libelosplugin.h
      └── lib/
         ├── elos/
         │  ├── backend/
         │  │  ├── backend_dummy.so
         │  │  ├── backend_json.so
         │  │  └── backend_sql.so
         │  └── scanner/
         │     ├── scanner_kmsg.so
         │     ├── scanner_shmem.so
         │     └── scanner_syslog.so
         ├── libelos.so -> libelos.so.0
         ├── libelos.so.0 -> libelos.so.0.52.3
         ├── libelos.so.0.52.3
         ├── libeloslog4c.so -> libeloslog4c.so.0
         ├── libeloslog4c.so.0 -> libeloslog4c.so.0.52.3
         ├── libeloslog4c.so.0.52.3
         ├── libelosplugin.so -> libelosplugin.so.0
         ├── libelosplugin.so.0 -> libelosplugin.so.0.52.3
         ├── libelosplugin.so.0.52.3
         └── test/
            └── elos unit tests
 

Dependencies
============

+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
|                   | elosd | libelos | mock_libelos | libelosplugin | scanner | backend   | Clients | Demos     | unit tests |
+===================+=======+=========+==============+===============+=========+===========+=========+===========+============+
| safu              | ✅    | ✅      | ✅           | ✅            | ✅      | ✅        | ✅      | ✅        | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| mock_safu         |       |         |              |               |         |           |         |           | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| samconf           | ✅    |         |              | ✅            | ✅      | ✅        | ✅      | ✅        | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| mock_samconf      |       |         |              |               |         |           |         |           | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| cmocka            |       |         | ✅           |               |         |           |         |           | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| cmocka_extensions |       |         | ✅           |               |         |           |         |           | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| cmocka_mocks      |       |         |              |               |         |           |         |           | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| asan [#f1]_       | ✅    | ✅      | ✅           | ✅            | ✅      | ✅        | ✅      | ✅        | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| esmtp             |       |         |              |               |         |           |         | ✅ [#f5]_ |            |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| jason-c           | ✅    | ✅      | ✅           | ✅            | ✅      | ✅        |         |           | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| libcurl           |       |         |              |               |         | ✅ [#f2]_ |         |           |            |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| libmnl            | ✅    |         |              |               |         |           |         |           | ✅         |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| log4c             |       |         |              |               |         |           |         | ✅ [#f6]_ |            |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| mongoc_shared     |       |         |              |               |         | ✅ [#f3]_ |         |           |            |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+
| sqlite3           |       |         |              |               |         | ✅ [#f4]_ |         |           |            |
+-------------------+-------+---------+--------------+---------------+---------+-----------+---------+-----------+------------+

.. [#f1] only for all Debug builds
.. [#f2] for the InfluxDB backend
.. [#f3] for the NoSQL backend
.. [#f4] for the SQL backend
.. [#f5] for elosMon
.. [#f6] for libeloslog4c and elos_log4c_demo


Options
=======

.. _cmake-options:

cmake options
-------------
- CMAKE_BUILD_TYPE: (default "Debug")
- ENABLE_CI: (default OFF) adds `-Werror` to the compile options
- UNIT_TESTS: (default ON) Build the unit tests
- INSTALL_UNIT_TESTS: (default ON) install the unit tests
- ELOSD_EVENTLOGGING_BACKEND_SQL: (default ON) Build the sqlite3 backend plugin
- ELOSD_EVENTLOGGING_BACKEND_NOSQL: (default OFF) Build the NoSQL backend plugin
- ELOSD_EVENTLOGGING_BACKEND_INFLUXDB: (default OFF) Build the InfluxDB backend plugin

.. _buildsh-options:

ci/build.sh options
-------------------
- [BUILD_TYPE] (default "Debug") sets the build type for cmake
- `--ci` enables `-DENABLE_CI` for cmake and implicitly sets `--clean --verbose`
- `--clean|-c` deletes the build directory before the build for a fresh start
- `--verbose|-v` adds `-v` to the parameters for ninja
- `--package` implicitly sets the `BUILD_TYPE` to `Release` and adds `-D PACKAGING=true` to cmake. Also implicitly sets `--clean`


CI-Helper scripts
=================

The `ci` folder contains a bunch of helper scripts to do different CI task.
Keeping them separated from a specific CI solution enables us to reuse the code
and additionally use them locally during development.

ci/trigger_jenkins_ci.sh
------------------------

This script can be used to trigger and monitor Jenkins pipelines. We use it to
trigger Yocto build jobs on a Jenkins instance.

.. program-output:: ./ci/trigger_jenkins_ci.sh --help
