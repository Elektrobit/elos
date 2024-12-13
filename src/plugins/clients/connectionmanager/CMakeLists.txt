# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_subdirectory(clientauthorizedprocesses)

create_interface_library(
  FROM
    connectionmanager
  LIBRARIES
    safu::safu_interface
)

add_library(
  connectionmanager_static
  STATIC
    private/clientblacklist.c
    private/clientconnection_worker.c
    private/clientconnection.c
    private/connectionmanager_listen.c
    private/connectionmanager_start.c
    private/connectionmanager_stop.c
    private/connectionmanager.c
)
set_property(TARGET connectionmanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  connectionmanager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)
target_link_libraries(
  connectionmanager_static
  PRIVATE
    eventbuffer_static
    elos_common_interface
  PUBLIC
    clientauthorizedprocesses_static
    tcp_clientauthorization_static
    messagehandler_static
    libelosplugin
    elos_common_interface
    eventbuffer_interface
    rpnfilter_interface
)
create_weak_library(
  FROM
    connectionmanager_static
  WEAKEN_DEPENDENCIES
    eventbuffer_static
    processfilter_static
)
