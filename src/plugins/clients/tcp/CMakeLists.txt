# SPDX-License-Identifier: MIT
find_package(samconf REQUIRED)

add_library(
  client_tcp SHARED
  private/tcp.c
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
    client_tcp_interface
    elos_common_interface
    libelosplugin
    samconf::samconf
)

target_include_directories(
  client_tcp
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS client_tcp DESTINATION ${ELOSD_CLIENT_PATH})
