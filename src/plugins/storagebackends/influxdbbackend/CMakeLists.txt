# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)
find_package(CURL REQUIRED)

add_library(
  backend_influxdb SHARED
  private/InfluxDbBackend.c
  private/InfluxDbBackendPlugin.c
)

create_interface_library(
  FROM
    backend_influxdb
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(backend_influxdb PROPERTIES PREFIX "")

target_include_directories(
  backend_influxdb
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  backend_influxdb
  PUBLIC
    backend_influxdb_interface
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    libelosplugin
    samconf::samconf
    safu::safu
    CURL::libcurl
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS backend_influxdb DESTINATION ${ELOSD_BACKEND_PATH})
endif()

create_static_library_from_shared(
  NAME
    backend_influxdb_static
  FROM
    backend_influxdb
  NO_PREFIX
)
