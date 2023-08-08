# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)

add_library(
  scanner_manager_static
  STATIC
    ./private/scanner_manager.c
)

target_include_directories(
  scanner_manager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  scanner_manager_static
  PRIVATE
    elos_common_interface
  PUBLIC
    version
    elos_event_static
    eventbuffer_static
    eventdispatcher_static
    eventlogging_static
    config_static
    eventlogging_static
    clientmanager_static
    elos_scanner_interface
    safu::safu
    ${CMAKE_DL_LIBS}
)

set_property(TARGET scanner_manager_static PROPERTY POSITION_INDEPENDENT_CODE ON)
add_compile_definitions(ELOSD_SCANNER_PATH="${ELOSD_SCANNER_PATH}")

create_weak_library(
  FROM
    scanner_manager_static
  WEAKEN_DEPENDENCIES
    elos_event_static
    eventbuffer_static
    eventdispatcher_static
    eventlogging_static
    config_static
    eventlogging_static
    clientmanager_static
)
