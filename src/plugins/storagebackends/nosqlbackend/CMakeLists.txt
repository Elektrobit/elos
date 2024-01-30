# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf)
find_package(mongoc-1.0 REQUIRED)

add_library(
  backend_nosql SHARED
  private/NoSqlBackend.c
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
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

target_link_libraries(
  backend_nosql
  PUBLIC
    backend_nosql_interface
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    libelosplugin
    plugincontrol_interface
    storagemanager_static
    samconf::samconf
    safu::safu
    mongo::mongoc_shared
)

install(TARGETS backend_nosql DESTINATION ${ELOSD_BACKEND_PATH})

create_static_library_from_shared(
  NAME
    backend_nosql_static
  FROM
    backend_nosql
  NO_PREFIX
)
