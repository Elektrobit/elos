# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_compile_definitions(SAFU_LOG="LOG")

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
    eloslog_static
    eventbuffer_static
    safu::safu
)

set_property(TARGET logger_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    logger_static
)
