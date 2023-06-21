# SPDX-License-Identifier: MIT
include(create_mock_lib.cmake)
find_package(cmocka_mocks)

create_mock_lib(
  NAME
    mock_safu
  SOURCES
    safu/mock_safu.c
    safu/mock_log.c
    safu/mock_vector.c
    safu/mock_ringbuffer.c
  INCLUDES
    $<TARGET_PROPERTY:safu,INCLUDE_DIRECTORIES>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/safu>
  DEFINITIONS
    MOCK_SAFU_LOG_MESSAGE_SIZE=512
  LIBRARIES
    cmocka_extensions::cmocka_extensions
  LIBRARIES_DYNAMIC
    safu
  LIBRARIES_STATIC
    safu_weak
  WRAP_FLAG
    MOCK_SAFU_FUNC_LINKER_WRAPPING
  WRAPS
    safuGetEnvOr
    safuReadFileToString
    safuWriteExactly
    safuRecvExactly
    safuSendExactly
    safuAllocMem
    safuJsonAddObject
    safuJsonAddNewInt
    safuJsonAddNewInt64
    safuJsonAddNewUint64
    safuJsonAddNewString
    safuJsonAddNewArray
    safuJsonAddNewArrayOfInitialSize
    safuJsonAddNewTimestamp
    safuJsonGetInt32
    safuJsonGetInt64
    safuJsonGetObject
    safuJsonGetArray
    safuJsonGetUint32
    safuJsonGetUint64
    safuJsonGetString
    safuJsonGetTimestamp
    safuTimeGetLocalTime
    safuLogFuncF
    safuLogFunc
    safuLogSetStreamLevel
    safuLogSetStream
    safuLogSetCallbackFunc
    safuLogSetPrefix
    safuVecCreate
    safuVecFree
    safuVecInsert
    safuVecRemove
    safuVecPush
    safuVecPop
    safuVecSet
    safuVecGet
    safuVecGetLast
    safuVecElements
    safuVecFind
    safuVecFindGet
    safuVecFindRemove
    safuGetHardwareId
    safuRingBufferNew
    safuRingBufferInitialize
    safuRingBufferDeleteMembers
    safuRingBufferDelete
    safuRingBufferWrite
    safuRingBufferRead
)

set_target_properties(
  mock_safu
  PROPERTIES
    SOVERSION ${SAFU_VERSION_MAJOR}
    VERSION ${SAFU_VERSION_MAJOR}.${SAFU_VERSION_MINOR}.${SAFU_VERSION_MICRO}
)

target_link_libraries(mock_safu PUBLIC safu)

target_link_options(
  mock_safu
  PUBLIC
   -Wl,--no-as-needed
   -Wl,--wrap=safuGetEnvOr
   -Wl,--wrap=safuReadFileToString
   -Wl,--wrap=safuWriteExactly
   -Wl,--wrap=safuRecvExactly
   -Wl,--wrap=safuSendExactly
   -Wl,--wrap=safuAllocMem
   -Wl,--wrap=safuJsonAddObject
   -Wl,--wrap=safuJsonAddNewInt
   -Wl,--wrap=safuJsonAddNewInt64
   -Wl,--wrap=safuJsonAddNewUint64
   -Wl,--wrap=safuJsonAddNewString
   -Wl,--wrap=safuJsonAddNewArray
   -Wl,--wrap=safuJsonAddNewArrayOfInitialSize
   -Wl,--wrap=safuJsonAddNewTimestamp
   -Wl,--wrap=safuJsonGetInt32
   -Wl,--wrap=safuJsonGetInt64
   -Wl,--wrap=safuJsonGetObject
   -Wl,--wrap=safuJsonGetArray
   -Wl,--wrap=safuJsonGetUint32
   -Wl,--wrap=safuJsonGetUint64
   -Wl,--wrap=safuJsonGetString
   -Wl,--wrap=safuJsonGetTimestamp
   -Wl,--wrap=safuTimeGetLocalTime
   -Wl,--wrap=safuLogFuncF
   -Wl,--wrap=safuLogFunc
   -Wl,--wrap=safuLogSetStreamLevel
   -Wl,--wrap=safuLogSetStream
   -Wl,--wrap=safuLogSetCallbackFunc
   -Wl,--wrap=safuLogSetPrefix
   -Wl,--wrap=safuVecCreate
   -Wl,--wrap=safuVecFree
   -Wl,--wrap=safuVecInsert
   -Wl,--wrap=safuVecRemove
   -Wl,--wrap=safuVecPush
   -Wl,--wrap=safuVecPop
   -Wl,--wrap=safuVecSet
   -Wl,--wrap=safuVecGet
   -Wl,--wrap=safuVecGetLast
   -Wl,--wrap=safuVecElements
   -Wl,--wrap=safuVecFind
   -Wl,--wrap=safuVecFindGet
   -Wl,--wrap=safuVecFindRemove
   -Wl,--wrap=safuGetHardwareId
   -Wl,--wrap=safuRingBufferNew
   -Wl,--wrap=safuRingBufferInitialize
   -Wl,--wrap=safuRingBufferDeleteMembers
   -Wl,--wrap=safuRingBufferDelete
   -Wl,--wrap=safuRingBufferWrite
   -Wl,--wrap=safuRingBufferRead
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "safu/Config.cmake.in"
  "safu/mock_safuConfig.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/safu
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/safu/mock_safuConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/safu/mock_safuConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/safu/mock_safuConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/safu"
)

install(TARGETS mock_safu EXPORT safuTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY safu/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/safu FILES_MATCHING PATTERN "*.h")
