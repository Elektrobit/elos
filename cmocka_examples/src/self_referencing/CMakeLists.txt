# SPDX-License-Identifier: MIT

add_library(
  selfref SHARED
	selfref.c
)
add_library(
	selfref_static STATIC
	selfref.c
)
set_property(TARGET selfref PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET selfref_static PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(selfref
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/>
)
target_include_directories(selfref_static
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/>
)

create_weak_library(FROM selfref_static)
