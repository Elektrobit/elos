# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_interface_library(
  FROM
    eventdispatcher
  LIBRARIES
    eventbuffer_interface
    eventprocessor_interface
    elos_event_interface
    safu::safu_interface
)

add_library(
  eventdispatcher_static
  STATIC
    ./private/eventdispatcher_dispatch.c
    ./private/eventdispatcher_worker.c
    ./private/eventdispatcher.c
)

target_include_directories(
  eventdispatcher_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  eventdispatcher_static
  PUBLIC
    eventdispatcher_interface
    eventprocessor_static
    eventbuffer_static
)

set_property(TARGET eventdispatcher_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    eventdispatcher_static
  WEAKEN_DEPENDENCIES
    eventprocessor_static
    eventbuffer_static
)
