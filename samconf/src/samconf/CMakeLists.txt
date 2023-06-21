# SPDX-License-Identifier: MIT
find_package(safu)

add_library(samconf_interface INTERFACE)
target_include_directories(samconf_interface
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interface>
    $<INSTALL_INTERFACE:include>
)
install(DIRECTORY interface/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(
  samconf SHARED
    private/samconf.c
    private/config_backend.c
    private/json_backend.c
    private/crypto_utils.c
    private/signature.c
    private/uri.c
)

set_property(TARGET samconf PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(
  samconf
  PROPERTIES
    SOVERSION ${SAMCONF_VERSION_MAJOR}
    VERSION ${SAMCONF_VERSION_MAJOR}.${SAMCONF_VERSION_MINOR}.${SAMCONF_VERSION_MICRO}
)

target_include_directories(
  samconf
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
  samconf
  PUBLIC
    samconf_interface
    safu::safu
    crypto
)

install(TARGETS samconf DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

create_static_library_from_shared(NAME samconf_static FROM samconf)
create_weak_library(FROM samconf)

install(
  EXPORT samconfTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/samconf
  NAMESPACE samconf::
  FILE samconfTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "samconfConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/samconf
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/samconfConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/samconfConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/samconfConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/samconf"
)

install(TARGETS samconf EXPORT samconfTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS samconf_interface EXPORT samconfTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
