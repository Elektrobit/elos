# SPDX-License-Identifier: MIT

# This functions was created to circumvent issues with OBJECT collections,
# like missing recompiles in incremental builds as well as weirdly behaved
# inheritance of target attributes like include directories causing headaches

# It takes a shared library target 'FROM', fetches the compiled objects as well
# as the target attributes and combines them into the static library 'NAME',
# this way the same objects are used and attributes like the include directories
# have to be specifed only once (for the shared library)

function(create_static_library_from_shared)
  cmake_parse_arguments(PARAM "NO_PREFIX" "NAME;FROM;PREFIX" "" ${ARGN})
  add_library(${PARAM_NAME} STATIC $<TARGET_OBJECTS:${PARAM_FROM}>)
  if(PARAM_NO_PREFIX)
    set_target_properties(${PARAM_NAME} PROPERTIES PREFIX "")
  elseif(PARAM_PREFIX)
    set_target_properties(${PARAM_NAME} PROPERTIES PREFIX "${PARAM_PREFIX}")
  endif()
  target_include_directories(${PARAM_NAME} PUBLIC $<TARGET_PROPERTY:${PARAM_FROM},INTERFACE_INCLUDE_DIRECTORIES>)
  target_compile_definitions(${PARAM_NAME} PUBLIC $<TARGET_PROPERTY:${PARAM_FROM},INTERFACE_COMPILE_DEFINITIONS>)
  get_target_property(target_linklibs ${PARAM_FROM} LINK_LIBRARIES)
  message(DEBUG "${target_linklibs}")
  if (target_linklibs)
    target_link_libraries(${PARAM_NAME} PUBLIC ${target_linklibs})
  endif()
endfunction()
