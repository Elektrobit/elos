# SPDX-License-Identifier: MIT

add_library(
  extref_library SHARED
  library.c
)
add_library(
	extref_library_static STATIC
  library.c
)


set_property(TARGET extref_library PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET extref_library_static PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(extref_library
  PUBLIC
	$<TARGET_PROPERTY:extref_dependency,INTERFACE_INCLUDE_DIRECTORIES>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
	$<INSTALL_INTERFACE:include/>
)
target_include_directories(extref_library_static
  PUBLIC
	$<TARGET_PROPERTY:extref_dependency_static,INTERFACE_INCLUDE_DIRECTORIES>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
	$<INSTALL_INTERFACE:include/>
)
