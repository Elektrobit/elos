# SPDX-License-Identifier: MIT
find_package(samconf REQUIRED)

add_library(
  client_dummy SHARED
  private/dummy.c
)

create_interface_library(
  FROM
    client_dummy
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(client_dummy PROPERTIES PREFIX "")

target_link_libraries(
  client_dummy
  PUBLIC
    client_dummy_interface
    elos_common_interface
    libelosplugin
    samconf::samconf
)

target_include_directories(
  client_dummy
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS client_dummy DESTINATION ${ELOSD_CLIENT_PATH})
