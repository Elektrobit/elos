# SPDX-License-Identifier: MIT

# Installs interface and public headers from the given components.
# Needed for shared libraries to work properly after installation.
function(install_component_headers)
  cmake_parse_arguments(PARAM "" "" "COMPONENTS" ${ARGN})
  foreach(COMPONENT IN LISTS PARAM_COMPONENTS)
    get_target_property(COMPONENT_INCLUDE_DIRS ${COMPONENT} INTERFACE_INCLUDE_DIRECTORIES)
    foreach(COMPONENT_INCLUDE_DIR IN LISTS COMPONENT_INCLUDE_DIRS)
      list(APPEND INCLUDE_DIRS ${COMPONENT_INCLUDE_DIR})
    endforeach()
  endforeach()
  list(REMOVE_DUPLICATES INCLUDE_DIRS)
  remove_install_interfaces(INCLUDE_DIRS)
  foreach(INCLUDE_DIR IN LISTS INCLUDE_DIRS)
    install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endforeach()
endfunction()

function(remove_install_interfaces LIST)
  set(REMOVED_LIST "")
  foreach(item ${${LIST}})
    if(item MATCHES "^\\$<INSTALL_INTERFACE:.*>$")
      continue()
    endif()
    list(APPEND REMOVED_LIST "${item}")
  endforeach()
  set(${LIST} ${REMOVED_LIST} PARENT_SCOPE)
endfunction(remove_install_interfaces)
