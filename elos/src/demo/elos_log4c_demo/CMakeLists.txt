# SPDX-License-Identifier: MIT
find_package(PkgConfig REQUIRED)
pkg_check_modules(LOG4C REQUIRED log4c)

add_library(
  libeloslog4c
  SHARED
    elos_appender.c
    elos_formatter.c
)

target_link_libraries(
  libeloslog4c
  PRIVATE
    log4c
    libelos
    version
)

set_target_properties(libeloslog4c PROPERTIES PREFIX "")
set_target_properties(
  libeloslog4c
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(TARGETS libeloslog4c DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_executable(
  elos_log4c_demo
  elos_log4c_demo.c
)

target_link_libraries(
  elos_log4c_demo
  PRIVATE
    log4c
    libeloslog4c
    libelos
    version
)


install(TARGETS elos_log4c_demo DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/log4crc
        DESTINATION ../${CMAKE_INSTALL_SYSCONFDIR}/elos/elos_log4c_demo)

