# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

create_interface_library(
  FROM
    elos_event
  LIBRARIES
    safu::safu_interface
)

add_library(
  elos_event_static
  STATIC
  private/event.c
  private/event_source.c
  private/event_vector.c
)

target_include_directories(
  elos_event_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  elos_event_static
  PUBLIC
    elos_common_interface
    elos_event_interface
    safu::safu
)

set_property(TARGET elos_event_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(FROM elos_event_static)
