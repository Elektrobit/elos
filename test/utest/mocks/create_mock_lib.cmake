# SPDX-License-Identifier: MIT

function(create_mock_lib)
  cmake_parse_arguments(PARSED_ARGS "NO_DYNAMIC_LIBRARY" "NAME;WRAP_FLAG" "SOURCES;INCLUDES;LIBRARIES;LIBRARIES_DYNAMIC;LIBRARIES_STATIC;DEFINITIONS;DEFINITIONS_DYNAMIC;DEFINITIONS_STATIC;WRAPS" ${ARGN})
  message(STATUS "create ${PARSED_ARGS_NAME} ${PARSED_SOURCES}")

  add_library(${PARSED_ARGS_NAME}_static STATIC ${PARSED_ARGS_SOURCES})
  if(PARSED_ARGS_DEFINITIONS OR DEFINITIONS_STATIC)
    target_compile_definitions(
      ${PARSED_ARGS_NAME}_static
      PRIVATE
        ${PARSED_ARGS_DEFINITIONS}
        ${PARSED_ARGS_DEFINITIONS_STATIC}
    )
  endif()
  target_link_libraries(
    ${PARSED_ARGS_NAME}_static
    PRIVATE
      cmocka
      ${PARSED_ARGS_LIBRARIES}
      ${PARSED_ARGS_LIBRARIES_STATIC}
  )
  target_include_directories(
    ${PARSED_ARGS_NAME}_static
    PUBLIC
      ${PARSED_ARGS_INCLUDES}
  )

  if(NOT ${PARSED_ARGS_NO_DYNAMIC_LIBRARY})
    add_library(${PARSED_ARGS_NAME} SHARED ${PARSED_ARGS_SOURCES})
    if(PARSED_ARGS_DEFINITIONS OR DEFINITIONS_DYNAMIC)
      target_compile_definitions(
        ${PARSED_ARGS_NAME}
        PRIVATE
          ${PARSED_ARGS_DEFINITIONS}
          ${PARSED_ARGS_DEFINITIONS_DYNAMIC}
      )
    endif()
    target_link_libraries(
      ${PARSED_ARGS_NAME}
      PRIVATE
        cmocka
        ${PARSED_ARGS_LIBRARIES}
        ${PARSED_ARGS_LIBRARIES_DYNAMIC}
    )
    target_include_directories(
      ${PARSED_ARGS_NAME}
      PUBLIC
        ${PARSED_ARGS_INCLUDES}
    )
    list(TRANSFORM PARSED_ARGS_WRAPS PREPEND "-Wl,--wrap=")
    target_link_options(
      ${PARSED_ARGS_NAME}
      PUBLIC
        -Wl,--no-as-needed
        ${PARSED_ARGS_WRAPS}
    )
    target_compile_definitions(
      ${PARSED_ARGS_NAME}
      PUBLIC
        ${PARSED_ARGS_WRAP_FLAG}
    )
  endif()

endfunction()
