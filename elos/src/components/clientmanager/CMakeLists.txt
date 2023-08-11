# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

create_interface_library(
  FROM
    clientmanager
  LIBRARIES
    eventbuffer_interface
    eventdispatcher_interface
    eventprocessor_interface
    processfilter_interface
    eventlogging_interface
    safu::safu_interface
)

add_library(
  clientmanager_static
  STATIC
    ./private/clientauthorization.c
    ./private/clientauthorizedprocesses.c
    ./private/clientblacklist.c
    ./private/clientmanager_connection.c
    ./private/clientmanager_listen.c
    ./private/clientmanager_start.c
    ./private/clientmanager_stop.c
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
    clientmanager_interface
    elos_common_interface
    elos_event_static
    config_static
    eventbuffer_static
    eventdispatcher_static
    eventprocessor_static
    processfilter_static
    eventlogging_static
    messagehandler_static
    safu::safu
    ${LIBMNL_LIBRARIES}
)

set_property(TARGET clientmanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    clientmanager_static
  WEAKEN_DEPENDENCIES
    elos_event_static
    config_static
    eventbuffer_static
    eventdispatcher_static
    eventprocessor_static
    processfilter_static
    eventlogging_static
    messagehandler_static
)
