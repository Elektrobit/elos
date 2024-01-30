# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

add_library(
  backend_json SHARED
  private/JsonBackend.c
  private/JsonBackendPlugin.c
)

create_interface_library(
  FROM
    backend_json
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(backend_json PROPERTIES PREFIX "")

target_include_directories(
  backend_json
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  backend_json
  PUBLIC
    backend_json_interface
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    libelosplugin
    plugincontrol_interface
    storagemanager_static
    samconf::samconf
    safu::safu
)

install(TARGETS backend_json DESTINATION ${ELOSD_BACKEND_PATH})

create_static_library_from_shared(
  NAME
    backend_json_static
  FROM
    backend_json
  NO_PREFIX
)
