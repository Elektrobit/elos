# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_interface_library(
  FROM
    scannermanager
  LIBRARIES
    elos_event_interface
    elos_event_static
    rpnfilter_static
    scannermanager_interface
    pluginmanager_interface
    samconf::samconf_interface
)


add_library(
  scannermanager_static
  STATIC
    ./private/scannermanager.c
)

target_include_directories(
  scannermanager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  scannermanager_static
  PUBLIC
    scannermanager_interface
    elos_common_interface
    pluginmanager_static
    config_static
    safu::safu
    samconf::samconf
)

set_property(TARGET scannermanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    scannermanager_static
  WEAKEN_DEPENDENCIES
    pluginmanager_static
    config_static
)
