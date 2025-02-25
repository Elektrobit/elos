# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_subdirectory(unix_clientauthorization)
add_subdirectory(unix_config)

add_library(
    client_unix
  SHARED
    private/unix.c
)
create_interface_library(
  FROM
    client_unix
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)
set_target_properties(client_unix PROPERTIES PREFIX "")
target_link_libraries(
  client_unix
  PUBLIC
    unix_clientauthorization_static
    unix_config_static
    libelosplugin
    libelosplugin_interface
    samconf::samconf
)
target_include_directories(
  client_unix
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS client_unix DESTINATION ${ELOSD_CLIENT_PATH})
endif()
