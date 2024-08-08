# SPDX-License-Identifier: MIT

find_package(json-c REQUIRED)
find_package(safu 0.56.2 REQUIRED)

add_library(
  libelos
  SHARED
    private/libelos.c
    private/libelos_communication.c
    private/libelos_constructor.c
    private/libelos_logger.c
)

target_include_directories(
  libelos
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private/
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public/>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
  libelos
  PRIVATE
    eloslog_static
  PUBLIC
    libelos_common
    json-c::json-c
    safu::safu
)

set_target_properties(libelos PROPERTIES PREFIX "")  # Prevent creation of "liblibelos.so"
set_target_properties(
  libelos
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(
  EXPORT libelosTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libelos
  NAMESPACE elos::
  FILE libelosTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "libelosConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/libelos
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libelosConfigVersion.cmake
  VERSION ${PKG_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libelosConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libelosConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/libelos"
)

install(TARGETS libelos EXPORT libelosTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

install(TARGETS libelos DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(${PROJECT_SOURCE_DIR}/cmake/pkgconfig/pkgconfig.cmake)
create_pkgconfig(
  NAME "elos"
  REQUIRES "safu >= ${safu_VERSION}, json-c >= 0.15, elos_common >= ${ELOS_VERSION}"
  LIBS "-lelos"
)

create_static_library_from_shared(
  NAME
    libelos_static
  FROM
    libelos
  NO_PREFIX
)

create_weak_library(
  FROM
    libelos
  TO
    libelos_static_weak
  WEAKEN_DEPENDENCIES
    elos_event_static
  NO_PREFIX
)
