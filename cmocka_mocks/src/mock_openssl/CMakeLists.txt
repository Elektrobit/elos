# SPDX-License-Identifier: MIT
find_package(cmocka_extensions REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(
  mock_openssl SHARED
  mock_openssl.c
)

set_property(TARGET mock_openssl PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(
  mock_openssl
  PROPERTIES
    SOVERSION ${CMOCKA_MOCKS_VERSION_MAJOR}
    VERSION ${CMOCKA_MOCKS_VERSION_MAJOR}.${CMOCKA_MOCKS_VERSION_MINOR}.${CMOCKA_MOCKS_VERSION_MICRO}
)

target_link_options(
  mock_openssl
  PUBLIC
    -Wl,--wrap=PEM_read_PUBKEY
    -Wl,--wrap=EVP_MD_CTX_new
    -Wl,--wrap=EVP_MD_CTX_create
    -Wl,--wrap=EVP_MD_fetch
    -Wl,--wrap=EVP_get_digestbyname
    -Wl,--wrap=EVP_DecodeBlock
    -Wl,--wrap=EVP_DigestVerifyInit
    -Wl,--wrap=EVP_DigestUpdate
    -Wl,--wrap=EVP_DigestVerifyFinal
    -Wl,--wrap=EVP_PKEY_free
    -Wl,--wrap=EVP_MD_free
    -Wl,--wrap=EVP_MD_CTX_free
    -Wl,--wrap=EVP_MD_CTX_destroy
)

target_include_directories(
  mock_openssl
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PUBLIC
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
  mock_openssl
  PRIVATE
    cmocka
    cmocka_extensions::cmocka_extensions
    OpenSSL::Crypto
)

install(
  EXPORT mock_opensslTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks
  NAMESPACE cmocka_mocks::
  FILE mock_opensslTargets.cmake # Not sure if this is still needed
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/mock_opensslConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mock_opensslConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks"
)

install(TARGETS mock_openssl EXPORT mock_opensslTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
