# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_subdirectory(tcp_clientauthorization)

add_compile_definitions(ELOSD_PORT=${ELOSD_PORT})
add_compile_definitions(ELOSD_INTERFACE="${ELOSD_INTERFACE}")
add_compile_definitions(ELOSD_CONNECTION_LIMIT=${ELOSD_CONNECTION_LIMIT})

add_library(
  messagehandler_static
  STATIC
    private/messagehandler/message_handler.c
    private/messagehandler/message_get_version.c
    private/messagehandler/message_event_publish.c
    private/messagehandler/message_event_subscribe.c
    private/messagehandler/message_event_unsubscribe.c
    private/messagehandler/message_log_find_event.c
    private/messagehandler/message_eventqueue_read.c
)
set_property(TARGET messagehandler_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  messagehandler_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)
target_link_libraries(
    messagehandler_static
    PRIVATE
      version
      eventbuffer_static
      eventfilter_static
    PUBLIC
      connectionmanager_interface
      libelosplugin
      safu::safu_interface
      elos_common_interface
      eventbuffer_interface
      rpnfilter_interface
)
create_weak_library(
  FROM
    messagehandler_static
  WEAKEN_DEPENDENCIES
    eventbuffer_static
)

add_library(
    client_tcp
  SHARED
    private/tcp.c
    private/tcpConfig.c
)

create_interface_library(
  FROM
    client_tcp
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(client_tcp PROPERTIES PREFIX "")

target_link_libraries(
  client_tcp
  PUBLIC
    messagehandler_static
    connectionmanager_static
    client_tcp_interface
    libelosplugin
    samconf::samconf
)

target_include_directories(
  client_tcp
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

target_compile_definitions(client_tcp PUBLIC ELOSD_PORT=${ELOSD_PORT})
target_compile_definitions(client_tcp PUBLIC ELOSD_INTERFACE="${ELOSD_INTERFACE}")

install(TARGETS client_tcp DESTINATION ${ELOSD_CLIENT_PATH})
