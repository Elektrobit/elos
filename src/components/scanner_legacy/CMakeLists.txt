# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_interface_library(
  FROM
    elos_scanner_legacy
  LIBRARIES
    eventbuffer_interface
    elos_event_interface
    safu::safu_interface
    samconf::samconf_interface
)
