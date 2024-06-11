# SPDX-License-Identifier: MIT

find_package(samconf 0.50.1 REQUIRED)
find_package(safu 0.50.1 REQUIRED)

create_interface_library(
  FROM
    libelosplugin
  LIBRARIES
    samconf::samconf_interface
    safu::safu_interface
  INSTALL
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
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)

target_link_libraries(
  libelosplugin
  PRIVATE
    elos_event_static
    eventfilter_static
  PUBLIC
    libelos_common
    libelosplugin_interface
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

install(
  EXPORT libelospluginTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libelosplugin
  NAMESPACE elos::
  FILE libelospluginTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "libelospluginConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/libelosplugin
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libelospluginConfigVersion.cmake
  VERSION ${PKG_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libelospluginConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libelospluginConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/libelosplugin"
)

install(TARGETS libelosplugin EXPORT libelospluginTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS libelosplugin_interface EXPORT libelospluginTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})


include(${PROJECT_SOURCE_DIR}/cmake/pkgconfig/pkgconfig.cmake)
create_pkgconfig(
  NAME "elosplugin"
  REQUIRES "safu >= ${safu_VERSION}, samconf >= ${samconf_VERSION}, libelos_common >= ${ELOS_VERSION}"
  LIBS "-lelosplugin"
)

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
  NO_PREFIX
)
