# SPDX-License-Identifier: MIT

find_package(safu 0.56.2 REQUIRED)

create_interface_library(
  FROM
    libelosdlt
  LIBRARIES
    safu::safu_interface
  INSTALL
)

add_library(
  libelosdlt
  SHARED
    private/libelosdlt.c
    private/extheader.c
    private/storageheader.c
    private/stdheader.c
    private/payload.c
    private/dltmapper/dltmapper.c
)

target_include_directories(
  libelosdlt
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
  PUBLIC
    $<INSTALL_INTERFACE:include/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public/>
)

target_link_libraries(
  libelosdlt
  PUBLIC
    libelos_common
    libelosdlt_interface
    safu::safu
)

set_target_properties(libelosdlt PROPERTIES PREFIX "")
set_target_properties(
  libelosdlt
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(
  EXPORT libelosdltTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libelosdlt
  NAMESPACE elos::
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "libelosdltConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/libelosdlt
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libelosdltConfigVersion.cmake
  VERSION ${PKG_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libelosdltConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libelosdltConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/libelosdlt"
)

install(TARGETS libelosdlt EXPORT libelosdltTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/)
install(TARGETS libelosdlt_interface EXPORT libelosdltTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

install(TARGETS libelosdlt DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(${PROJECT_SOURCE_DIR}/cmake/pkgconfig/pkgconfig.cmake)
create_pkgconfig(
  NAME "elosdlt"
  REQUIRES "safu >= ${safu_VERSION}"
  LIBS "-lelosdlt"
)

create_static_library_from_shared(
  NAME
    libelosdlt_static
  FROM
    libelosdlt
  NO_PREFIX
)

create_weak_library(
  FROM
    libelosdlt
  TO
    libelosdlt_static_weak
  NO_PREFIX
)
