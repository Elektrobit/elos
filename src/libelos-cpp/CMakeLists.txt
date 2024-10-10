# SPDX-License-Identifier: MIT
find_package(safu 0.57.0 REQUIRED)

enable_language(CXX)

add_library(
  libelos-cpp
  SHARED 
    private/libelos-cpp.cpp
)

target_include_directories(
  libelos-cpp
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private/
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public/>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
  libelos-cpp
  PRIVATE
    libelos
)

set_target_properties(libelos-cpp PROPERTIES PREFIX "")
set_target_properties(
  libelos-cpp
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(
  EXPORT libelos-cppTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libelos-cpp
  NAMESPACE elos::
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "libelos-cppConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/libelos-cpp
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libelos-cppConfigVersion.cmake
  VERSION ${PKG_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libelos-cppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libelos-cppConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/libelos-cpp"
)

install(TARGETS libelos-cpp EXPORT libelos-cppTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

install(TARGETS libelos-cpp DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(${PROJECT_SOURCE_DIR}/cmake/pkgconfig/pkgconfig.cmake)
create_pkgconfig(
  NAME "elos-cpp"
  REQUIRES "safu >= ${safu_VERSION},  elos >= ${ELOS_VERSION}"
  LIBS "-lelos-cpp"
)

create_static_library_from_shared(
  NAME
    libelos-cpp_static
  FROM
    libelos-cpp
  NO_PREFIX
)
