# SPDX-License-Identifier: MIT
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

if (ELOSD_SYSTEMD)
  pkg_check_modules(libsystemd REQUIRED libsystemd)
endif()


add_compile_definitions(SAFU_LOG=1)

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

if (ELOSD_SYSTEMD)
  target_compile_definitions(elosd PRIVATE ELOSD_SYSTEMD)
  target_link_libraries(
    elosd
    PUBLIC
      ${libsystemd_LIBRARIES}
  )

  target_include_directories(
    elosd
    PUBLIC
      ${libsystemd_INCLUDE_DIRS}
  )
endif()

if (INSTALL_ELOS_DAEMON)
  install(TARGETS elosd DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
