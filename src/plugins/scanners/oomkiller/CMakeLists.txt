# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

add_library(
  scanner_oomkiller SHARED
  private/OomKillerScanner.c
  private/OomKillerScannerPlugin.c
)

create_interface_library(
  FROM
    scanner_oomkiller
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(scanner_oomkiller PROPERTIES PREFIX "")

target_link_libraries(
  scanner_oomkiller
  PUBLIC
    scanner_oomkiller_interface
    elos_common_interface
    eventlogging_interface
    libelosplugin
    eventfilter_static
    samconf::samconf
    safu::safu
)

target_include_directories(
  scanner_oomkiller
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS scanner_oomkiller DESTINATION ${ELOSD_SCANNER_PATH})
endif()
