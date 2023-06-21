# SPDX-License-Identifier: MIT

function(create_interface_library)
  cmake_parse_arguments(PARAM "INSTALL" "FROM" "LIBRARIES" ${ARGN})
  set(interface_target_name "${PARAM_FROM}_interface")

  add_library(
    ${interface_target_name}
    INTERFACE
  )

  if (PARAM_LIBRARIES)
    target_link_libraries(
      ${interface_target_name}
      INTERFACE
        ${PARAM_LIBRARIES}
    )
  endif()

  target_include_directories(
    ${interface_target_name}
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interface>
      $<INSTALL_INTERFACE:include>
  )

  if (PARAM_INSTALL)
    install(DIRECTORY interface/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endif()
endfunction()
