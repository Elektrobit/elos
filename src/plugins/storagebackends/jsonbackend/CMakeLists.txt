# SPDX-License-Identifier: MIT
find_package(safu)
find_package(samconf)

add_library(
  backend_json SHARED
  ./JsonBackend.c
  ./JsonBackendPlugin.c
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
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    plugin_interface
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
