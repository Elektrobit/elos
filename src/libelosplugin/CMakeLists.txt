# SPDX-License-Identifier: MIT
include(../../cmake/shared_library.cmake)

find_package(samconf 0.50.1 REQUIRED)
find_package(safu 0.50.1 REQUIRED)

create_interface_library(
  FROM
  libelosplugin
  LIBRARIES
    elos_event_interface
    samconf::samconf_interface
    safu::safu_interface
)

add_library(
  libelosplugin
  SHARED
    private/libelosplugin.c
)

target_include_directories(
  libelosplugin
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_link_libraries(
  libelosplugin
  PUBLIC
    libelosplugin_interface
    elos_event_static
    samconf::samconf
    safu::safu
)

set_target_properties(libelosplugin PROPERTIES PREFIX "")  # Prevent creation of "liblibelos.so"
set_target_properties(
  libelosplugin
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(TARGETS libelosplugin DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install_component_headers(COMPONENTS elos_event_static)

create_static_library_from_shared(
  NAME
    libelosplugin_static
  FROM
    libelosplugin
  NO_PREFIX
)

create_weak_library(
  FROM
    libelosplugin
  TO
    libelosplugin_static_weak
  WEAKEN_DEPENDENCIES
    elos_event_static
  NO_PREFIX
)
