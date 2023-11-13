# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_interface_library(
  FROM
    clientmanager
  LIBRARIES
    rpnfilter_interface
    eventlogging_interface
    pluginmanager_interface
    elos_event_interface
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
    elos_event_static
    pluginmanager_static
    config_static
    safu::safu
    samconf::samconf
    eventfilter_static
)

set_property(TARGET clientmanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    clientmanager_static
  WEAKEN_DEPENDENCIES
    elos_event_static
    eventfilter_static
    pluginmanager_static
    config_static
)
