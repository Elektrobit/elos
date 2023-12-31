# SPDX-License-Identifier: MIT
include(interface_library.cmake)
include(static_from_shared.cmake)
include(weak_library.cmake)

set(ELOSD_PORT "54321")
set(ELOSD_INTERFACE "0.0.0.0")
set(ELOSD_CONNECTION_LIMIT 200)
set(ELOSD_CONFIG_PATH ${CMAKE_INSTALL_FULL_SYSCONFDIR}/elos)
set(ELOSD_CONFIG_FILE "/etc/elos/elosd.json")
set(ELOSD_SCANNER_PATH ${CMAKE_INSTALL_FULL_LIBDIR}/elos/scanner)
set(ELOSD_CLIENT_PATH ${CMAKE_INSTALL_FULL_LIBDIR}/elos/client)
set(ELOSD_BACKEND_PATH ${CMAKE_INSTALL_FULL_LIBDIR}/elos/backend)
set(ELOSD_HARDWAREID_FILE "/etc/machine-id")
set(ELOSD_HARDWAREID_MAX_LENGTH 2048)
set(ELOSD_HARDWAREID_DEFAULT_VALUE "INVALID")
set(ELOSD_HARDWAREID_ENVIRONMENT_PREFIX "ELOSD_HARDWAREID_")

set(ELOSD_LOG_PREFIX "[ELOSD] ")
set(ELOSD_LOG_FILTER "")
set(ELOSD_LOG_LEVEL "Debug")

add_subdirectory(clients)
add_subdirectory(common)
add_subdirectory(components)
add_subdirectory(demos)
add_subdirectory(elosd)
add_subdirectory(libelos)
add_subdirectory(libelosplugin)
add_subdirectory(plugins)
add_subdirectory(version)
