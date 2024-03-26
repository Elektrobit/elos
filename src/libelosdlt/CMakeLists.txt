# SPDX-License-Identifier: MIT
include(../../cmake/shared_library.cmake)

find_package(safu 0.50.1 REQUIRED)

create_interface_library(
  FROM
    libelosdlt
  LIBRARIES
    safu::safu_interface
  INSTALL
)

add_library(
  libelosdlt
  SHARED
    private/libelosdlt.c
)

target_include_directories(
  libelosdlt
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

target_link_libraries(
  libelosdlt
  PUBLIC
    libelosdlt_interface
    safu::safu
)

set_target_properties(libelosdlt PROPERTIES PREFIX "")
set_target_properties(
  libelosdlt
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(TARGETS libelosdlt DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(${PROJECT_SOURCE_DIR}/cmake/pkgconfig/pkgconfig.cmake)
create_pkgconfig(
  NAME "elosdlt"
  REQUIRES "safu >= ${safu_VERSION}"
  LIBS "-lelosdlt"
)

create_static_library_from_shared(
  NAME
    libelosdlt_static
  FROM
    libelosdlt
  NO_PREFIX
)

create_weak_library(
  FROM
    libelosdlt
  TO
    libelosdlt_static_weak
  NO_PREFIX
)
