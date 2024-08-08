# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)
find_package(mongoc-1.0 REQUIRED)

add_library(
  backend_nosql SHARED
  private/NoSqlBackend.c
  private/NoSqlBackendPlugin.c
)

create_interface_library(
  FROM
    backend_nosql
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(backend_nosql PROPERTIES PREFIX "")

target_include_directories(
  backend_nosql
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  backend_nosql
  PUBLIC
    backend_nosql_interface
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    libelosplugin
    samconf::samconf
    safu::safu
    mongo::mongoc_shared
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS backend_nosql DESTINATION ${ELOSD_BACKEND_PATH})
endif()

create_static_library_from_shared(
  NAME
    backend_nosql_static
  FROM
    backend_nosql
  NO_PREFIX
)
