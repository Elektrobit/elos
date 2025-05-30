# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_compile_definitions(ELOSD_SOCKET_PATH="${ELOSD_SOCKET_PATH}")
add_compile_definitions(ELOSD_CONNECTION_LIMIT=${ELOSD_CONNECTION_LIMIT})

add_library(
  unix_config_static
  STATIC
    private/unixConfig.c
)
set_target_properties(unix_config_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(unix_config_static PROPERTIES PREFIX "")
target_link_libraries(
  unix_config_static
  PUBLIC
    libelosplugin
    libelosplugin_interface
    samconf::samconf
)
target_include_directories(
  unix_config_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_compile_definitions(unix_config_static PUBLIC ELOSD_SOCKET_PATH="${ELOSD_SOCKET_PATH}")
target_compile_definitions(unix_config_static PUBLIC ELOSD_CONNECTION_LIMIT=${ELOSD_CONNECTION_LIMIT})

create_weak_library(
  FROM
    unix_config_static
)