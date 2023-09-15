# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.21)
include(cmake/ConfigureLibrary.cmake)
include(cmake/project.cmake)

project(elos LANGUAGES C VERSION ${ELOS_VERSION})

list(APPEND CMAKE_PREFIX_PATH "${INSTALL_DIR}/usr/local/lib/cmake")

configureLibrary(cmocka_extensions ${CMOCKA_EXTENSIONS_URI} ${CMOCKA_EXTENSIONS_REF})
configureLibrary(cmocka_mocks ${CMOCKA_MOCKS_URI} ${CMOCKA_MOCKS_REF})
configureLibrary(safu ${SAFU_URI} ${SAFU_REF})
configureLibrary(samconf ${SAMCONF_URI} ${SAMCONF_REF})

project_set_environment()
project_set_version_variables()
project_add_documentation_target(
  TITLE
    "BaseOS ElosD Documentation"
  MARKDOWN
    ${CMAKE_SOURCE_DIR}/documentation/documentation.md
)

option(ELOSD_EVENTLOGGING_BACKEND_SQL "Build sqlite3 bakend plugin" ON)
option(ELOSD_EVENTLOGGING_BACKEND_NOSQL "Build NoSQL backend plugin" OFF)
option(ELOSD_EVENTLOGGING_BACKEND_INFLUXDB "Build InfluxDB backend plugin" OFF)

add_subdirectory(src)
if(UNIT_TESTS)
  enable_testing()
  add_subdirectory(test/utest)
  add_subdirectory(test/tsan)
endif(UNIT_TESTS)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(PACKAGING)
  include(cmake/Packing.cmake)
endif(PACKAGING)

