# SPDX-License-Identifier: MIT
add_library(
  scanner_kmsg SHARED
  private/kmsg.c
  private/kmsg_mapper.c
)

create_interface_library(
  FROM
    scanner_kmsg
  LIBRARIES
    libelos_common
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(scanner_kmsg PROPERTIES PREFIX "")

target_link_libraries(
  scanner_kmsg
  PUBLIC
    libelos_common
    libelosplugin
    samconf::samconf
    scanner_kmsg_interface
)

target_include_directories(
    scanner_kmsg
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


if (INSTALL_ELOS_PLUGINS)
  install(TARGETS scanner_kmsg DESTINATION ${ELOSD_SCANNER_PATH})
endif()

create_static_library_from_shared(
  NAME
    scanner_kmsg_static
  FROM
    scanner_kmsg
  NO_PREFIX
)

create_weak_library(
  FROM
    scanner_kmsg
  TO
    scanner_kmsg_static_weak
  WEAKEN_DEPENDENCIES
    scanner_kmsg_static
  NO_PREFIX
)
