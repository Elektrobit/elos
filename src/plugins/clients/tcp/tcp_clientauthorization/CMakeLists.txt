# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

add_library(
  tcp_clientauthorization_static
  STATIC
    private/clientauthorization.c
)
set_property(TARGET tcp_clientauthorization_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  tcp_clientauthorization_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)
target_link_libraries(
  tcp_clientauthorization_static
  PRIVATE
    eventbuffer_static
    elos_common_interface
  PUBLIC
    clientauthorizedprocesses_static
    libelosplugin
    elos_common_interface
    eventbuffer_interface
    rpnfilter_interface
    ${LIBMNL_LIBRARIES}
)
