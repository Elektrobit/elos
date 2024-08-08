# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

create_interface_library(
  FROM
    eventfilter
  LIBRARIES
    rpnfilter_interface
    safu::safu_interface
)

add_library(
  eventfilter_static
  STATIC
    private/eventfilter_builder_tokenize.c
    private/eventfilter_builder_compile.c
    private/eventfilter_execute.c
    private/eventfilter.c
)

target_include_directories(
  eventfilter_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  eventfilter_static
  PUBLIC
    eventfilter_interface
    elos_common_interface
    elos_event_static
    rpnfilter_static
)

set_property(TARGET eventfilter_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
  eventfilter_static
  WEAKEN_DEPENDENCIES
    elos_event_static
    rpnfilter_static
)
