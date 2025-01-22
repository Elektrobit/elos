# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

add_compile_definitions(SAFU_LOG=1)

create_interface_library(
  FROM
    eventbuffer
  LIBRARIES
    elos_common_interface
    elos_event_interface
    safu::safu_interface
)

add_library(
  eventbuffer_static
  STATIC
    ./private/eventbuffer.c
)

target_include_directories(
  eventbuffer_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  eventbuffer_static
  PUBLIC
    eventbuffer_interface
    elos_event_static
    safu::safu
)

set_property(TARGET eventbuffer_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    eventbuffer_static
  WEAKEN_DEPENDENCIES
    elos_event_static
)
