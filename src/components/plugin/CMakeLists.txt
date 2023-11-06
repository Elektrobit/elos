# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_interface_library(
  FROM
    plugin
  LIBRARIES
    libelosplugin_interface
    elos_common_interface
    samconf::samconf_interface
    safu::safu_interface
)

add_library(
  plugin_static
  STATIC
    ./private/plugin_load.c
    ./private/plugin_vector.c
    ./private/plugin_workerthread.c
    ./private/plugin.c
)

target_include_directories(
  plugin_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  plugin_static
  PUBLIC
    plugin_interface
    samconf::samconf
    safu::safu
)

set_property(TARGET plugin_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    plugin_static
)
