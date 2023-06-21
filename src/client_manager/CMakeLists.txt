# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

create_interface_library(
  FROM
    clientmanager
  LIBRARIES
    eventprocessor_interface
    processfilter_interface
    eventlogging_interface
    safu::safu_interface
)

add_library(
  clientmanager_static
  STATIC
    ./private/client_authorization.c
    ./private/client_authorized_processes.c
    ./private/client_blacklist.c
    ./private/client_manager_threads.c
    ./private/client_manager_start.c
    ./private/client_manager_stop.c
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
    eventprocessor_static
    processfilter_static
    eventlogging_static
    messagehandler_static
)
