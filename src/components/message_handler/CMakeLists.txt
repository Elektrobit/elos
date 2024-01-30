# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)

add_library(
  messagehandler_static
  STATIC
    ./private/message_handler.c
    ./private/message_get_version.c
    ./private/message_event_publish.c
    ./private/message_event_subscribe.c
    ./private/message_event_unsubscribe.c
    ./private/message_log_find_event.c
    ./private/message_eventqueue_read.c
)

target_include_directories(
  messagehandler_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  messagehandler_static
  PUBLIC
    connectionmanager_interface
    elos_common_interface
    eventbuffer_static
    eventprocessor_static
    eventlogging_static
    elos_event_static
    logger_static
    safu::safu
    version
)

set_property(TARGET messagehandler_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    messagehandler_static
  WEAKEN_DEPENDENCIES
    eventbuffer_static
    eventprocessor_static
    eventlogging_static
    elos_event_static
)
