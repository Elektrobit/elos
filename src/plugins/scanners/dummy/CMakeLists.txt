# SPDX-License-Identifier: MIT
find_package(samconf 0.50.1 REQUIRED)

add_library(
  scanner_dummy SHARED
  private/dummy.c
)

create_interface_library(
  FROM
    scanner_dummy
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(scanner_dummy PROPERTIES PREFIX "")

target_link_libraries(
  scanner_dummy
  PUBLIC
    scanner_dummy_interface
    elos_common_interface
    libelosplugin
    samconf::samconf
)

target_include_directories(
  scanner_dummy
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS scanner_dummy DESTINATION ${ELOSD_SCANNER_PATH})
endif()
