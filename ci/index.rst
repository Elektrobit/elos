
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

Build Confugruations
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

Indendet to easier install the dependencies `cmocka_extensions`, `cmocka_mocks`, `safu` & `samconf`

.. code-block::

   ci/install_deps.py

By default it installes from the main branch of the GitHub repositories.
The behavior can be modified by putting a `dependencies.json` in the root of the project.

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
A path is used as is without checking the branch, tag or commit and includes local changes that aren't commited jet.

These dependencies get installed into `build/deps/` and if provided with an URL the sources can be found in `build/deps/src`.

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
| libmml            | ✅    |         |              |               |         |           |         |           | ✅         |
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
- ELOSD_EVENTLOGGING_BACKEND_SQL: (default ON) Build the sqlite3 bakend plugin
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

cmocka_extensions, cmocka_mocks, safu & samconf install options
---------------------------------------------------------------

cmocka_extensions cmocka_mocks, safu and samconf can be installed just like any other library in a system include path or provided to the build via the environment variable `CMAKE_PREFIX_PATH`.
But for now it is also possible to let the build system get them via `FetchContent`_

In ci/dependenccy_sources.sh
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- SOURCES_URI: (default "https://github.com/Elektrobit/") the base url under which to find the repos.
- DEFAULT_BRANCH: (default "integration") the branch to use if nothing else is specified
- CMOCKA_EXTENSIONS_REPO_NAME: (default "cmocka_extensions.git") the repository name for cmocka_extensons
- CMOCKA_EXTENSIONS_REPO_PATH: (default "${SOURCES_URI}/${CMOCKA_EXTENSIONS_REPO_NAME}") the full url to get cmocka_extensions
- CMOCKA_EXTENSIONS_REPO_REF: (default "${DEFAULT_BRANCH}") the branch to use for cmocka_extensions
- CMOCKA_MOCKS_REPO_NAME: (default "cmocka_mocks.git") the repository name for cmocka_mocks
- CMOCKA_MOCKS_REPO_PATH: (default "${SOURCES_URI}/${CMOCKA_MOCKS_REPO_NAME}") the full url for cmocka_mocks
- CMOCKA_MOCKS_REPO_REF: (default ${DEFAULT_BRANCH}") the branch to use for cmocka_mocks
- SAFU_REPO_NAME: (default "safu.git") the repository name for safu
- SAFU_REPO_PATH: (default "${SOURCES_URI}/${SAFU_REPO_NAME}") the full url for safu
- SAFU_REPO_REF: (default "${DEFAULT_BRANCH}") the branch to use for safu
- SAMCONF_REPO_NAME: (default "samconf.git") the repository name for samconf
- SAMCONF_REPO_PATH: (default "${SOURCES_URI}/${SAMCONF_REPO_NAME}") the full url for samconf
- SAMCONF_REPO_REF: (default "${DEFAULT_BRANCH}") the branche to use for samconf

From those options the cmake parameter are generated to install `cmocka_extensions`, `mocka_mocks`, `safu` and `samconf` in the cmake call directly using `FetchContent`_

- `-D CMOCKA_EXTENSIONS_URI=${CMOCKA_EXTENSIONS_REPO_PATH}`
- `-D CMOCKA_EXTENSIONS_REF=${CMOCKA_EXTENSIONS_REPO_REF}`
- `-D CMOCKA_MOCKS_URI=${CMOCKA_MOCKS_REPO_PATH}`
- `-D CMOCKA_MOCKS_REF=${CMOCKA_MOCKS_REPO_REF}`
- `-D SAFU_URI=${SAFU_REPO_PATH}`
- `-D SAFU_REF=${SAFU_REPO_REF}`
- `-D SAMCONF_URI=${SAMCONF_REPO_PATH}`
- `-D SAMCONF_REF=${SAMCONF_REPO_REF}`
 
