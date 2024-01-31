# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)

create_interface_library(
  FROM
    rpnfilter
  LIBRARIES
    safu::safu_interface
)

add_library(
  rpnfilter_static
  STATIC
    ./private/rpnfilter_builder_compile.c
    ./private/rpnfilter_builder_finalize.c
    ./private/rpnfilter_builder_tokenize.c
    ./private/rpnfilter_builder.c
    ./private/rpnfilter_execute.c
    ./private/rpnfilter_vector.c
    ./private/rpnfilter.c
)

target_include_directories(
  rpnfilter_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  rpnfilter_static
  PUBLIC
    elos_common_interface
    rpnfilter_interface
    safu::safu
)

set_property(TARGET rpnfilter_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(FROM rpnfilter_static)
