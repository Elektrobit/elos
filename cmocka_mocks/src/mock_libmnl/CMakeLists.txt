# SPDX-License-Identifier: MIT
find_package(cmocka_extensions REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

add_library(
  mock_libmnl SHARED
  mock_libmnl.c
)

set_property(TARGET mock_libmnl PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(
  mock_libmnl
  PROPERTIES
    SOVERSION ${CMOCKA_MOCKS_VERSION_MAJOR}
    VERSION ${CMOCKA_MOCKS_VERSION_MAJOR}.${CMOCKA_MOCKS_VERSION_MINOR}.${CMOCKA_MOCKS_VERSION_MICRO}
)

target_link_options(
  mock_libmnl
  PUBLIC
    -Wl,--wrap=mnl_socket_open
    -Wl,--wrap=mnl_socket_bind
    -Wl,--wrap=mnl_socket_close
    -Wl,--wrap=mnl_socket_recvfrom
    -Wl,--wrap=mnl_socket_sendto
    -Wl,--wrap=mnl_socket_get_portid
)

target_include_directories(
  mock_libmnl
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PUBLIC
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
  mock_libmnl
  PRIVATE
    cmocka
    cmocka_extensions::cmocka_extensions
    ${LIBMNL_LIBRARIES}
)

install(
  EXPORT mock_libmnlTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks
  NAMESPACE cmocka_mocks::
  FILE mock_libmnlTargets.cmake # Not sure if this is still needed
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/mock_libmnlConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mock_libmnlConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks"
)

install(TARGETS mock_libmnl EXPORT mock_libmnlTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
