# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf)

if(ELOSD_EVENTLOGGING_BACKEND_SQL)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SQLITE3 REQUIRED sqlite3)
endif()


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
    plugincontrol_interface
    storagemanager_static
    samconf::samconf
    safu::safu
    $<$<BOOL:${ELOSD_EVENTLOGGING_BACKEND_SQL}>:${SQLITE3_LIBRARIES}>
)

install(TARGETS backend_sql DESTINATION ${ELOSD_BACKEND_PATH})

create_static_library_from_shared(
  NAME
    backend_sql_static
  FROM
    backend_sql
  NO_PREFIX
)
