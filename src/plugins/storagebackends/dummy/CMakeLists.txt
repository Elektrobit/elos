# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

add_library(
  backend_dummy SHARED
  private/dummyBackend.c
)

set_target_properties(backend_dummy PROPERTIES PREFIX "")

target_link_libraries(
  backend_dummy
  PUBLIC
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    libelosplugin
    plugincontrol_interface
    storagemanager_static
    samconf::samconf
    safu::safu
)

install(TARGETS backend_dummy DESTINATION ${ELOSD_BACKEND_PATH})
