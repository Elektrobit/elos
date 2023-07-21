# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_interface_library(
  FROM
    eloslog
  LIBRARIES
    elos_event_interface
)

add_library(
  eloslog_static
  STATIC
    ./private/eloslog.c
)

target_include_directories(
  eloslog_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  eloslog_static
  PUBLIC
    eloslog_interface
    elos_event_interface
    eventprocessor_static
    safu::safu
)

set_property(TARGET eloslog_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    eloslog_static
)
