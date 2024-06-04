# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

add_library(
  client_oomkiller SHARED
  private/OomKilerScanner.c
  private/OomKillerScannerPlugin.c
)

create_interface_library(
  FROM
    client_oomkiller
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(client_oomkiller PROPERTIES PREFIX "")

target_link_libraries(
  client_oomkiller
  PUBLIC
    client_oomkiller_interface
    elos_common_interface
    eventlogging_interface
    libelosplugin
    eventfilter_static
    samconf::samconf
    safu::safu
)

target_include_directories(
  client_oomkiller
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS client_oomkiller DESTINATION ${ELOSD_CLIENT_PATH})
endif()
