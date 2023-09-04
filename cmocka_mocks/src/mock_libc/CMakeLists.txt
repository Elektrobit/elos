# SPDX-License-Identifier: MIT
find_package(cmocka_extensions REQUIRED)

add_library(
  mock_libc SHARED
  mock_libc.c
)

set_property(TARGET mock_libc PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(
  mock_libc
  PROPERTIES
    SOVERSION ${CMOCKA_MOCKS_VERSION_MAJOR}
    VERSION ${CMOCKA_MOCKS_VERSION_MAJOR}.${CMOCKA_MOCKS_VERSION_MINOR}.${CMOCKA_MOCKS_VERSION_MICRO}
)

target_include_directories(
  mock_libc
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PUBLIC
    $<INSTALL_INTERFACE:include/>
)

target_link_options(
 mock_libc
 PUBLIC
    -Wl,--wrap=malloc
    -Wl,--wrap=realloc
    -Wl,--wrap=calloc
    -Wl,--wrap=free
    -Wl,--wrap=memcpy
    -Wl,--wrap=strdup
    -Wl,--wrap=fopen
    -Wl,--wrap=fdopen
    -Wl,--wrap=fclose
    -Wl,--wrap=fwrite
    -Wl,--wrap=fread
    -Wl,--wrap=fseek
    -Wl,--wrap=scandir
    -Wl,--wrap=access
    -Wl,--wrap=fseek
    -Wl,--wrap=rewind
    -Wl,--wrap=ftell
    -Wl,--wrap=popen
    -Wl,--wrap=pclose
    -Wl,--wrap=getline
    -Wl,--wrap=getenv
    -Wl,--wrap=getpid
    -Wl,--wrap=opendir
    -Wl,--wrap=closedir
    -Wl,--wrap=remove
    -Wl,--wrap=readdir
    -Wl,--wrap=stat
    -Wl,--wrap=fputc
    -Wl,--wrap=time
    -Wl,--wrap=clock_gettime
    -Wl,--wrap=inet_aton
    -Wl,--wrap=inet_pton
    -Wl,--wrap=getaddrinfo
    -Wl,--wrap=freeaddrinfo
    -Wl,--wrap=socket
    -Wl,--wrap=setsockopt
    -Wl,--wrap=accept
    -Wl,--wrap=connect
    -Wl,--wrap=bind
    -Wl,--wrap=listen
    -Wl,--wrap=close
    -Wl,--wrap=open
    -Wl,--wrap=regerror
    -Wl,--wrap=regexec
    -Wl,--wrap=regcomp
    -Wl,--wrap=strndup
    -Wl,--wrap=raise
    -Wl,--wrap=pselect
    -Wl,--wrap=pthread_create
    -Wl,--wrap=pthread_join
    -Wl,--wrap=pthread_once
    -Wl,--wrap=pthread_mutex_init
    -Wl,--wrap=pthread_mutex_destroy
    -Wl,--wrap=pthread_mutex_trylock
    -Wl,--wrap=pthread_mutex_lock
    -Wl,--wrap=pthread_mutex_unlock
    -Wl,--wrap=pthread_mutex_timedlock
    -Wl,--wrap=sem_init
    -Wl,--wrap=sem_post
    -Wl,--wrap=sem_destroy
    -Wl,--wrap=sem_timedwait
    -Wl,--wrap=eventfd
    -Wl,--wrap=eventfd_read
    -Wl,--wrap=eventfd_write
)

target_link_libraries(
  mock_libc
  PRIVATE
    cmocka
    cmocka_extensions::cmocka_extensions
)

install(
  EXPORT mock_libcTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks
  NAMESPACE cmocka_mocks::
  FILE mock_libcTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/mock_libcConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mock_libcConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks"
)

install(TARGETS mock_libc EXPORT mock_libcTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
