# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

create_interface_library(
  FROM
    connectionmanager
  LIBRARIES
    eventbuffer_interface
    eventdispatcher_interface
    eventprocessor_interface
    processfilter_interface
    eventlogging_interface
    safu::safu_interface
)

add_library(
  connectionmanager_static
  STATIC
    ./private/clientauthorization.c
    ./private/clientauthorizedprocesses.c
    ./private/clientblacklist.c
    ./private/clientconnection_worker.c
    ./private/clientconnection.c
    ./private/connectionmanager_listen.c
    ./private/connectionmanager_start.c
    ./private/connectionmanager_stop.c
    ./private/connectionmanager.c
)

target_include_directories(
  connectionmanager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  connectionmanager_static
  PUBLIC
    connectionmanager_interface
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

set_property(TARGET connectionmanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    connectionmanager_static
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
