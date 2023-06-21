# SPDX-License-Identifier: MIT
include(create_mock_lib.cmake)
find_package(cmocka_mocks)
find_package(safu)

create_mock_lib(
  NAME
    mock_samconf
  SOURCES
    samconf/mock_samconf.c
  INCLUDES
    $<TARGET_PROPERTY:samconf,INCLUDE_DIRECTORIES>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/samconf>
  LIBRARIES
    cmocka_extensions::cmocka_extensions
  LIBRARIES_STATIC
    samconf_weak
  WRAP_FLAG
    MOCK_SAMCONF_USE_WRAP
  WRAPS
    samconfConfigGet
    samconfConfigGetBool
    samconfConfigGetInt32
    samconfConfigGetString
    samconfGetBackendOps
    samconfVerifySignature
    samconfConfigBackendNew
    samconfConfigBackendInit
    samconfConfigBackendDelete
    samconfConfigDelete
    samconfConfigDeleteMembers
    samconfConfigNew
    samconfConfigAdd
    samconfJsonBackendSupports
    samconfJsonBackendOpen
    samconfJsonBackendLoad
    samconfJsonBackendClose
    samconfLookupBackend
    samconfUriNew
    samconfLoadPublicKey
    samconfCryptoUtilsVerify
    samconfUriPattern
    samconfUriDelete
    samconfBase64Decode
    samconfUriInit
    samconfUriDeleteMembers
)

target_link_libraries(
  mock_samconf
  PUBLIC
    samconf
)

set_target_properties(
  mock_samconf
  PROPERTIES
    SOVERSION ${SAMCONF_VERSION_MAJOR}
    VERSION ${SAMCONF_VERSION_MAJOR}.${SAMCONF_VERSION_MINOR}.${SAMCONF_VERSION_MICRO}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "samconf/Config.cmake.in"
  "samconf/mock_samconfConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/samconf
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/samconf/mock_samconfConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/samconf/mock_samconfConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/samconf/mock_samconfConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/samconf"
)

install(TARGETS mock_samconf EXPORT samconfTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY samconf/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/samconf FILES_MATCHING PATTERN "*.h")
