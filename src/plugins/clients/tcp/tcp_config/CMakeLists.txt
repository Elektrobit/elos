# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

add_compile_definitions(ELOSD_PORT=${ELOSD_PORT})
add_compile_definitions(ELOSD_INTERFACE="${ELOSD_INTERFACE}")
add_compile_definitions(ELOSD_CONNECTION_LIMIT=${ELOSD_CONNECTION_LIMIT})

add_library(
  tcp_config_static
  STATIC
    private/tcpConfig.c
)
set_target_properties(tcp_config_static PROPERTIES PREFIX "")
target_link_libraries(
  tcp_config_static
  PUBLIC
    libelosplugin
    samconf::samconf
)
target_include_directories(
  tcp_config_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_compile_definitions(tcp_config_static PUBLIC ELOSD_PORT=${ELOSD_PORT})
target_compile_definitions(tcp_config_static PUBLIC ELOSD_INTERFACE="${ELOSD_INTERFACE}")
target_compile_definitions(tcp_config_static PUBLIC ELOSD_CONNECTION_LIMIT=${ELOSD_CONNECTION_LIMIT})
