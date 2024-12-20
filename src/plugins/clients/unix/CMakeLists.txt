# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

add_subdirectory(unix_clientauthorization)
add_subdirectory(unix_config)

add_library(
  unix_connectionmanager_static
  STATIC
    private/connectionmanager/connectionmanager_start.c
    private/connectionmanager/connectionmanager_stop.c
    private/connectionmanager/connectionmanager.c
)
set_property(TARGET unix_connectionmanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  unix_connectionmanager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)
target_link_libraries(
  unix_connectionmanager_static
  PRIVATE
    processfilter_static
    eventbuffer_static
    elos_common_interface
    messagehandler_static
  PUBLIC
    libelosplugin
    processfilter_interface
    elos_common_interface
    eventbuffer_interface
    rpnfilter_interface
    ${LIBMNL_LIBRARIES}
)
create_weak_library(
  FROM
    unix_connectionmanager_static
  WEAKEN_DEPENDENCIES
    eventbuffer_static
    processfilter_static
)

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
    unix_connectionmanager_static
    unix_config_static
    client_unix_interface
    libelosplugin
    samconf::samconf
)

target_include_directories(
  client_unix
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS client_unix DESTINATION ${ELOSD_CLIENT_PATH})
endif()
