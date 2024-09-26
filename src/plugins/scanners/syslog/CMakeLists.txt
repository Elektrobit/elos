# SPDX-License-Identifier: MIT
add_library(
  scanner_syslog SHARED
  private/syslog.c
  private/logline_mapper.c
)

create_interface_library(
  FROM
    scanner_syslog
  LIBRARIES
    elos_event_interface
    safu::safu_interface
    samconf::samconf_interface
)

set_target_properties(scanner_syslog PROPERTIES PREFIX "")

target_link_libraries(
  scanner_syslog
  PRIVATE
    eventfilter_static
  PUBLIC
    libelosplugin
    rpnfilter_static
    scanner_syslog_interface
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

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS scanner_syslog DESTINATION ${ELOSD_SCANNER_PATH})
endif()
