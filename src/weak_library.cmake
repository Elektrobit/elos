# SPDX-License-Identifier: MIT

# Attention: Using this function in the <project>/test/utest folder will cause
#   errors like "json-c::json-c not found". To save everyone the headache of
#   searching and adding every missing "find_package()" call, the weak targets
#   have to be made in the same place where the original is created for now;
#   This is usually from within the <project>/src/CMakeLists.txt file.

function(create_weak_library)
  cmake_parse_arguments(PARAM "NO_PREFIX" "FROM;TO;PREFIX;HELPER_SCRIPT" "WEAKEN_DEPENDENCIES" ${ARGN})

  if (NOT PARAM_TO)
    set(weak_target_name "${PARAM_FROM}_weak")
  else()
    set(weak_target_name "${PARAM_TO}")
  endif()

  if(PARAM_NO_PREFIX)
    set(weak_target_prefix "")
  elseif(PARAM_PREFIX)
    set(weak_target_prefix "${PARAM_PREFIX}")
  else()
    set(weak_target_prefix "lib")
  endif()

  if(NOT PARAM_HELPER_SCRIPT)
    set(PARAM_HELPER_SCRIPT ${PROJECT_SOURCE_DIR}/test/tools/create_weak_library.sh)
  endif()

  set(weak_target_from ${PARAM_FROM})
  get_target_property(weak_target_binary_dir ${PARAM_FROM} BINARY_DIR)
  set(weak_target_binary "${weak_target_binary_dir}/${weak_target_prefix}${weak_target_name}.a" )

  message(STATUS "Create weak library ${weak_target_name}")

  add_custom_command(
    OUTPUT ${weak_target_binary}
    DEPENDS ${weak_target_from}
    COMMAND ${PARAM_HELPER_SCRIPT} ${weak_target_binary} "$<TARGET_OBJECTS:${weak_target_from}>"
    WORKING_DIRECTORY ${weak_target_binary_dir}
    VERBATIM
  )
  add_library(${weak_target_name} STATIC IMPORTED GLOBAL)
  add_custom_target(${weak_target_name}_builder DEPENDS ${weak_target_binary})
  add_dependencies(${weak_target_name} ${weak_target_name}_builder)

  set_target_properties(${weak_target_name} PROPERTIES IMPORTED_LOCATION "${weak_target_binary}")
  target_include_directories(${weak_target_name} INTERFACE $<TARGET_PROPERTY:${weak_target_from},INTERFACE_INCLUDE_DIRECTORIES>)
  target_compile_definitions(${weak_target_name} INTERFACE $<TARGET_PROPERTY:${weak_target_from},INTERFACE_COMPILE_DEFINITIONS>)
  get_target_property(weak_target_linklibs ${weak_target_from} LINK_LIBRARIES)
  if (weak_target_linklibs)
    foreach(X IN LISTS PARAM_WEAKEN_DEPENDENCIES)
      list(REMOVE_ITEM weak_target_linklibs ${X})
      list(APPEND weak_target_linklibs ${X}_weak)
    endforeach()
    target_link_libraries(${weak_target_name} INTERFACE ${weak_target_linklibs})
  endif()
endfunction()
