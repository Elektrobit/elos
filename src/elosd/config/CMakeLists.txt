# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.73.1 REQUIRED)

create_interface_library(
  FROM
    config
)

add_library(
  config_static
  STATIC
    ./private/config.c
)

target_include_directories(
  config_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

add_compile_definitions(ELOSD_CONFIG_PATH="${ELOSD_FULL_CONFIG_PATH}")

add_compile_definitions(ELOSD_SCANNER_PATH="${ELOSD_SCANNER_PATH}")
add_compile_definitions(ELOSD_CLIENT_PATH="${ELOSD_CLIENT_PATH}")
add_compile_definitions(ELOSD_BACKEND_PATH="${ELOSD_BACKEND_PATH}")
add_compile_definitions(ELOSD_LOG_FILTER="${ELOSD_LOG_FILTER}")
add_compile_definitions(ELOSD_LOG_LEVEL="${ELOSD_LOG_LEVEL}")
add_compile_definitions(ELOSD_RUN_DIR="${ELOSD_RUN_DIR}")

target_link_libraries(
  config_static
  PUBLIC
    config_interface
    elos_common_interface
    safu::safu
    samconf::samconf
)

set_property(TARGET config_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    config_static
)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/elosd.json
  DESTINATION
    ${ELOSD_CONFIG_PATH}
)
