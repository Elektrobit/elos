# SPDX-License-Identifier: MIT
add_library(
  scanner_syslog SHARED
  syslog.c
  logline_mapper.c
)

set_target_properties(scanner_syslog PROPERTIES PREFIX "")

target_link_libraries(
  scanner_syslog
  PRIVATE
    scanner_interface
    eventfilter_static
  PUBLIC
    rpnfilter_static
    elos_common_interface
    elos_event_static
    samconf::samconf
    config_static
)

target_include_directories(
    scanner_syslog
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS scanner_syslog DESTINATION ${ELOSD_SCANNER_PATH})
