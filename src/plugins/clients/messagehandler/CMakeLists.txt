# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_library(
  messagehandler_static
  STATIC
    private/message_handler.c
    private/message_get_version.c
    private/message_event_publish.c
    private/message_event_subscribe.c
    private/message_event_unsubscribe.c
    private/message_log_find_event.c
    private/message_eventqueue_read.c
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
