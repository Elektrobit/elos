# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf REQUIRED)

add_library(
  logger_static
  STATIC
    ./private/logger.c
)

target_include_directories(
  logger_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  logger_static
  PUBLIC
    elos_common_interface
    samconf::samconf_interface
    eventprocessor_static
    eloslog_static
    eventbuffer_static
    safu::safu
)

set_property(TARGET logger_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    logger_static
)
