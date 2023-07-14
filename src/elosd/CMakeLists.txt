# SPDX-License-Identifier: MIT
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(safu REQUIRED)
find_package(samconf REQUIRED)

if(ELOSD_EVENTLOGGING_BACKEND_SQL)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SQLITE3 REQUIRED sqlite3)
endif()

if(ELOSD_EVENTLOGGING_BACKEND_NOSQL)
  find_package(mongoc-1.0 REQUIRED)
endif()

option(ELOSD_EVENTLOGGING_BACKEND_SQL "Build elosd with SQL event logging backend instead (PoC!!!)" OFF)
option(ELOSD_EVENTLOGGING_BACKEND_NOSQL "Build elosd with NoSQL event logging backend instead (PoC!!!)" OFF)

add_executable(
  elosd
  main.c
)

add_compile_definitions(ELOSD_PORT=${ELOSD_PORT})
add_compile_definitions(ELOSD_INTERFACE="${ELOSD_INTERFACE}")
add_compile_definitions(ELOSD_CONFIG_PATH="${ELOSD_CONFIG_PATH}")
add_compile_definitions(ELOSD_CONFIG_FILE="${ELOSD_CONFIG_FILE}")
add_compile_definitions(ELOSD_SCANNER_PATH="${ELOSD_SCANNER_PATH}")
add_compile_definitions(ELOSD_BACKEND_PATH="${ELOSD_BACKEND_PATH}")
add_compile_definitions(ELOSD_LOG_PREFIX="${ELOSD_LOG_PREFIX}")

target_link_libraries(
  elosd
  PUBLIC
    version
    elos_event_static
    rpnfilter_static
    eventbuffer_static
    eventfilter_static
    eventprocessor_static
    eventlogging_static
    config_static
    pluginmanager_static
    eventlogging_static
    clientmanager_static
    messagehandler_static
    scanner_manager_static
    elos_scanner_interface
    Threads::Threads
    safu::safu
    samconf::samconf
)

install(TARGETS elosd DESTINATION ${CMAKE_INSTALL_BINDIR})
