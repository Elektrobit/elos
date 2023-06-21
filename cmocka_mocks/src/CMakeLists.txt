# SPDX-License-Identifier: MIT

add_subdirectory(mock_libc)
add_subdirectory(mock_libmnl)
add_subdirectory(mock_jsonc)
add_subdirectory(mock_openssl)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "Config.cmake.in"
  "cmocka_mocksConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cmocka_mocksConfig.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks"
)
