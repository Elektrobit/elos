# SPDX-License-Identifier: MIT
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_executable(
  elosd
  main.c
)

add_subdirectory(clientmanager)
add_subdirectory(config)
add_subdirectory(eventdispatcher)
add_subdirectory(eventlogging)
add_subdirectory(eventprocessor)
add_subdirectory(plugincontrol)
add_subdirectory(pluginmanager)
add_subdirectory(scannermanager)
add_subdirectory(storagemanager)

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
    libelos_common
    eventbuffer_static
    eventdispatcher_static
    eventprocessor_static
    logger_static
    eloslog_static
    config_static
    pluginmanager_static
    eventlogging_static
    clientmanager_static
    scannermanager_static
    Threads::Threads
    safu::safu
    samconf::samconf
)

if (INSTALL_ELOS_DAEMON)
  install(TARGETS elosd DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
