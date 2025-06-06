# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)


add_library(
  backend_sql SHARED
  private/SqlBackend.c
  private/SqlBackendPlugin.c
)

set_target_properties(backend_sql PROPERTIES PREFIX "")

target_include_directories(
  backend_sql
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

target_link_libraries(
  backend_sql
  PUBLIC
    eventlogging_interface
    elos_common_interface
    eventfilter_static
    libelosplugin
    samconf::samconf
    safu::safu
    $<$<BOOL:${ELOSD_EVENTLOGGING_BACKEND_SQL}>:${SQLITE3_LIBRARIES}>
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS backend_sql DESTINATION ${ELOSD_BACKEND_PATH})
endif()

create_static_library_from_shared(
  NAME
    backend_sql_static
  FROM
    backend_sql
  NO_PREFIX
)
