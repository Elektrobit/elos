# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_interface_library(
  FROM
    pluginmanager
  LIBRARIES
    plugincontrol_interface
)

add_library(
  pluginmanager_static
  STATIC
    ./private/pluginmanager_entry.c
    ./private/pluginmanager_load.c
    ./private/pluginmanager_remove.c
    ./private/pluginmanager_unload.c
    ./private/pluginmanager.c
)

target_include_directories(
  pluginmanager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  pluginmanager_static
  PUBLIC
    libelosplugin_static
    pluginmanager_interface
    elos_common_interface
    safu::safu
    plugincontrol_static
)

set_property(TARGET pluginmanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    pluginmanager_static
)
