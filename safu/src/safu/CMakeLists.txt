# SPDX-License-Identifier: MIT
find_package(json-c REQUIRED)

add_library(safu_interface INTERFACE)
target_include_directories(safu_interface
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interface>
    $<INSTALL_INTERFACE:include>
)
install(DIRECTORY interface/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(
  safu SHARED
  private/common.c
  private/vector.c
  private/json.c
  private/time.c
  private/log.c
  private/ringbuffer.c
)

set_property(TARGET safu PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(
  safu
  PUBLIC
  SAFU_HARDWAREID_FILE="${SAFU_HARDWAREID_FILE}"
  SAFU_HARDWAREID_MAX_LENGTH=${SAFU_HARDWAREID_MAX_LENGTH}
  SAFU_HARDWAREID_DEFAULT_VALUE="${SAFU_HARDWAREID_DEFAULT_VALUE}"
  SAFU_HARDWAREID_ENVIRONMENT_PREFIX="${SAFU_HARDWAREID_ENVIRONMENT_PREFIX}"
)

set_target_properties(
  safu
  PROPERTIES
    SOVERSION ${SAFU_VERSION_MAJOR}
    VERSION ${SAFU_VERSION_MAJOR}.${SAFU_VERSION_MINOR}.${SAFU_VERSION_MICRO}
)

target_include_directories(safu
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
  safu
  PUBLIC
    safu_interface
    json-c::json-c
)

install(TARGETS safu DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

create_static_library_from_shared(NAME safu_static FROM safu)
create_weak_library(FROM safu)

install(
  EXPORT safuTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/safu
  NAMESPACE safu::
  FILE safuTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "safuConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/safu
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/safuConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/safuConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/safuConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/safu"
)

install(TARGETS safu EXPORT safuTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS safu_interface EXPORT safuTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})