# SPDX-License-Identifier: MIT

add_library(
  extref_dependency SHARED
  dependency.c
)
add_library(
	extref_dependency_static STATIC
  dependency.c
)

set_property(TARGET extref_dependency PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET extref_dependency_static PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(extref_dependency
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
	$<INSTALL_INTERFACE:include/>
)
target_include_directories(extref_dependency_static
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
	$<INSTALL_INTERFACE:include/>
)
create_weak_library(FROM extref_dependency_static)
