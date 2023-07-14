# SPDX-License-Identifier: MIT

create_interface_library(
  FROM
    processfilter
  LIBRARIES
    rpnfilter_interface
)

add_library(
    processfilter_static
  STATIC
    private/processfilter_builder_tokenize.c
    private/processfilter_builder_compile.c
    private/processfilter_execute.c
    private/processfilter.c
)

target_include_directories(
    processfilter_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
    processfilter_static
  PUBLIC
    processfilter_interface
    elos_common_interface
    rpnfilter_static
)

set_property(TARGET processfilter_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    processfilter_static
  WEAKEN_DEPENDENCIES
    rpnfilter_static
)
