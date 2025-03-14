# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMNL REQUIRED libmnl)

create_interface_library(
  FROM
  tcp_clientauthorization
  LIBRARIES
    safu::safu_interface
)

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
)
target_link_libraries(
  tcp_clientauthorization_static
  PRIVATE
    eventbuffer_static
    libelosplugin
  PUBLIC
    libelosplugin_interface
    elos_common_interface
    eventbuffer_interface
    rpnfilter_interface
    ${LIBMNL_LIBRARIES}
)

create_weak_library(
  FROM
    tcp_clientauthorization_static
)
