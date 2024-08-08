# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_interface_library(
  FROM
    clientmanager
  LIBRARIES
    eventlogging_interface
    pluginmanager_interface
    samconf::samconf_interface
)


add_library(
  clientmanager_static
  STATIC
    ./private/clientmanager.c
)

target_include_directories(
  clientmanager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  clientmanager_static
  PUBLIC
    eventlogging_interface
    clientmanager_interface
    elos_common_interface
    pluginmanager_static
    config_static
    safu::safu
    samconf::samconf
)

set_property(TARGET clientmanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    clientmanager_static
  WEAKEN_DEPENDENCIES
    pluginmanager_static
    config_static
)
