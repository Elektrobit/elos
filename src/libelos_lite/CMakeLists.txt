# SPDX-License-Identifier: MIT

add_library(
  libelos-lite
  STATIC
    private/libelos_lite.c
    private/libelos_lite_publish.c
    private/message_utils.c
)

target_include_directories(
  libelos-lite
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private/
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public/>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/interface>
    $<INSTALL_INTERFACE:include/>
)

set_target_properties(libelos-lite PROPERTIES PREFIX "")  # Prevent creation of "liblibelos-lite.so"
set_target_properties(
  libelos-lite
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)
target_compile_definitions(libelos-lite PRIVATE ELOS_VERSION="${ELOS_VERSION}")

if (DEFINED ELOSLITE_SEND_BUFFER_SIZE)
  target_compile_definitions(libelos-lite PRIVATE ELOSLITE_SEND_BUFFER_SIZE=${ELOSLITE_SEND_BUFFER_SIZE})
endif ()

install(TARGETS libelos-lite DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
  FILES
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/interface/elos/event/event_message_codes.h>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/interface/elos/event/event_classification.h>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/interface/elos/event/event_severity.h>
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/elos/event/
)


install(
  EXPORT libelos-liteTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libelos-lite
  NAMESPACE elos::
  FILE libelos-liteTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "libelos-liteConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/libelos-lite
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libelos-liteConfigVersion.cmake
  VERSION ${PKG_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libelos-liteConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libelos-liteConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/libelos-lite"
)

install(TARGETS libelos-lite EXPORT libelos-liteTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

include(${PROJECT_SOURCE_DIR}/cmake/pkgconfig/pkgconfig.cmake)
create_pkgconfig(
  NAME "elos-lite"
  LIBS "-lelos-lite"
)

