# SPDX-License-Identifier: MIT
add_subdirectory(elos_log4c_demo)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBESMTP REQUIRED libesmtp-1.0 )

add_executable(
  demo_libelos_v2
  demo_libelos_v2.c
)

target_link_libraries(
  demo_libelos_v2
  PRIVATE
    libelos
    version
)

install(TARGETS demo_libelos_v2 DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(
  syslog_example
  syslog.c
)

target_link_libraries(
  syslog_example
  PRIVATE
    libelos
    version
)

install(TARGETS syslog_example DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(
  elosc
  elosc.c
)

target_link_libraries(
  elosc
  PRIVATE
    libelos
    version
)

install(TARGETS elosc DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(
  tinyElosc
  tinyElosc.c
)

target_link_libraries(
  tinyElosc
  PRIVATE
    elos_common_interface
    libelos
    version
)

install(TARGETS tinyElosc DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(
  elosMon
  elosMon.c
)

target_link_libraries(
  elosMon
  PRIVATE
    elos_common_interface
    esmtp
    libelos
    version
)

install(TARGETS elosMon DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(
  demo_eventbuffer
  demo_eventbuffer.c
)

target_link_libraries(
  demo_eventbuffer
  PRIVATE
    eventbuffer_static
)

if(CMAKE_BUILD_TYPE STREQUAL "Tsan")
  target_link_libraries(demo_eventbuffer PRIVATE tsan)
  target_compile_options(demo_eventbuffer PUBLIC "-fsanitize=thread")
endif()

install(TARGETS demo_eventbuffer DESTINATION ${CMAKE_INSTALL_BINDIR})
