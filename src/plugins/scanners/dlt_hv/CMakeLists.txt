# SPDX-License-Identifier: MIT
find_package(samconf 0.56.3 REQUIRED)

add_library(
  scanner_dlt_hv SHARED
  private/dlt_hv.c
  private/shmem_ring_buffer.c
  private/utils.c
  private/dlt_scanner.c
)

create_interface_library(
  FROM
    scanner_dlt_hv
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(scanner_dlt_hv PROPERTIES PREFIX "")

target_link_libraries(
  scanner_dlt_hv
  PUBLIC
    scanner_dlt_hv_interface
    elos_common_interface
    libelosplugin
    samconf::samconf
)

target_include_directories(
  scanner_dlt_hv
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS scanner_dlt_hv DESTINATION ${ELOSD_SCANNER_PATH})
endif()
