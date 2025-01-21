# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_library(
  clientauthorizedprocesses_static
  STATIC
    private/clientauthorizedprocesses.c
)
set_property(TARGET clientauthorizedprocesses_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  clientauthorizedprocesses_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)
target_link_libraries(
  clientauthorizedprocesses_static
  PRIVATE
    processfilter_static
  PUBLIC
    processfilter_interface
)
