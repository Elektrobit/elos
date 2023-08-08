# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

create_interface_library(
  FROM
    elos_scanner
  LIBRARIES
    eventbuffer_interface
    elos_event_interface
    safu::safu_interface
    samconf::samconf_interface
)
