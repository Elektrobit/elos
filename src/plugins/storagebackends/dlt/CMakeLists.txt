# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

add_library(
  backend_dlt_logger SHARED
  private/dltLoggerBackend.c
  private/dltLoggerBackendPlugin.c
)

set_target_properties(backend_dlt_logger PROPERTIES PREFIX "")

target_link_libraries(
  backend_dlt_logger
  PUBLIC
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    libelosplugin
    libelosdlt
    samconf::samconf
    safu::safu
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS backend_dlt_logger DESTINATION ${ELOSD_BACKEND_PATH})
endif()
