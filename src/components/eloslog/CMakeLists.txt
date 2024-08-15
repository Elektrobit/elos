# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_library(
  eloslog_static
  STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/private/eloslog.c
)

target_include_directories(
  eloslog_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  eloslog_static
  PUBLIC
    elos_event_static
    safu::safu
)

set_property(TARGET eloslog_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    eloslog_static
)
