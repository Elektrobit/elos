# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_subdirectory(tcp_clientauthorization)
add_subdirectory(tcp_config)

add_library(
    client_tcp
  SHARED
    private/tcp.c
    private/tcp_connectionmanager.c
)
create_interface_library(
  FROM
    client_tcp
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)
set_target_properties(client_tcp PROPERTIES PREFIX "")
target_link_libraries(
  client_tcp
  PUBLIC
    tcp_clientauthorization_static
    tcp_config_static
    client_tcp_interface
    libelosplugin
    samconf::samconf
)
target_include_directories(
  client_tcp
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)
install(TARGETS client_tcp DESTINATION ${ELOSD_CLIENT_PATH})
