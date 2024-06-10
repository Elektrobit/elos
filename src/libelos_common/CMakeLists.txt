# SPDX-License-Identifier: MIT

add_library(
  libelos_common
  SHARED
    $<TARGET_OBJECTS:elos_event_static>
    $<TARGET_OBJECTS:eventfilter_static>
    $<TARGET_OBJECTS:rpnfilter_static>
)
set_property(TARGET libelos_common PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  libelos_common
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/common/interface>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/interface>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/eventfilter/interface>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/rpnfilter/interface>
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/public>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/eventfilter/public>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/rpnfilter/public>
)
set_target_properties(libelos_common PROPERTIES PREFIX "")  # Prevent creation of "liblibelos_common.so"
set_target_properties(
  libelos_common
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(
  TARGETS libelos_common
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
  DIRECTORY
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/common/interface/>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/interface/>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/eventfilter/interface/>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/rpnfilter/interface/>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/event/public/>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/eventfilter/public/>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/components/rpnfilter/public/>
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
  EXPORT libelos_commonTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libelos_common
  NAMESPACE elos::
  FILE libelos_commonTargets.cmake # Not sure if this is still needed
)


include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "libelos_commonConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/libelos_common
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)
install(TARGETS libelos_common EXPORT libelos_commonTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libelos_commonConfigVersion.cmake
  VERSION ${PKG_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libelos_commonConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libelos_commonConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/libelos_common"
)

include(${PROJECT_SOURCE_DIR}/cmake/pkgconfig/pkgconfig.cmake)
create_pkgconfig(
  NAME "elos_common"
  REQUIRES "safu >= ${safu_VERSION}, samconf >= ${samconf_VERSION}"
  LIBS "-lelos_common"
)

create_weak_library(
  FROM
    libelos_common
  TO
    libelos_common_static_weak
  NO_PREFIX
)
