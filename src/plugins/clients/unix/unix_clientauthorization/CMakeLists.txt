# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_library(
  unix_clientauthorization_static
  STATIC
    private/clientauthorization.c
)
set_property(TARGET unix_clientauthorization_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  unix_clientauthorization_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)
target_link_libraries(
  unix_clientauthorization_static
  PRIVATE
    eventbuffer_static
    elos_common_interface
  PUBLIC
    libelosplugin_interface
    libelosplugin
    elos_common_interface
    eventbuffer_interface
    rpnfilter_interface
)
