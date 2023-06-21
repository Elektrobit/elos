# SPDX-License-Identifier: MIT

add_library(
  cmocka_extensions INTERFACE
)

set_property(TARGET cmocka_extensions PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(cmocka_extensions
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
  cmocka_extensions
  INTERFACE
    cmocka
)

install(
  EXPORT cmocka_extensionsTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_extensions
  NAMESPACE cmocka_extensions::
  FILE cmocka_extensionsTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "cmocka_extensionsConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_extensions
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/cmocka_extensionsConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cmocka_extensionsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmocka_extensionsConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_extensions"
)

install(TARGETS cmocka_extensions EXPORT cmocka_extensionsTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
