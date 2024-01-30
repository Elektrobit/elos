# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_interface_library(
  FROM
    eventlogging
  LIBRARIES
    rpnfilter_interface
    elos_event_interface
    safu::safu_interface
    samconf::samconf_interface
)


add_library(
  eventlogging_static
  STATIC
    ./private/LogAggregator.c
)

target_include_directories(
  eventlogging_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  eventlogging_static
  PUBLIC
    eventlogging_interface
    elos_common_interface
    elos_event_static
    config_static
    storagemanager_static
    safu::safu
    samconf::samconf
    eventfilter_static
)

set_property(TARGET eventlogging_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    eventlogging_static
  WEAKEN_DEPENDENCIES
    elos_event_static
    eventfilter_static
)
