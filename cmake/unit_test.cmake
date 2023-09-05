# SPDX-License-Identifier: MIT
if(NOT CMAKE_CROSSCOMPILING)
  find_package(cmocka 1.1.5)
  # get cmocka
  if(NOT cmocka_FOUND)
    message(STATUS "CMocka >=1.1.5 was not found, building static version")
    include(CMakeModules/FetchCMocka.cmake)
    set(CMOCKA_LIBRARIES cmocka-static)
  endif()
else()
  find_package(cmocka 1.1.5 REQUIRED)
endif()
find_package(cmocka_extensions REQUIRED)

function(create_unit_test)
  cmake_parse_arguments(PARSED_ARGS "" "NAME" "SOURCES;INCLUDES;LIBRARIES;DEFINITIONS" ${ARGN})
  get_filename_component(UNIT_DIR ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)
  get_filename_component(UNIT_NAME ${UNIT_DIR} NAME)

  message(STATUS "Create unit test ${PARSED_ARGS_NAME} for ${UNIT_NAME}")
  add_executable(${PARSED_ARGS_NAME} ${PARSED_ARGS_SOURCES})

  target_link_libraries(
    ${PARSED_ARGS_NAME}
    PRIVATE
    ${PARSED_ARGS_LIBRARIES}
    ${CMOCKA_LIBRARIES}
    cmocka_extensions::cmocka_extensions
  )

  target_include_directories(
    ${PARSED_ARGS_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../..
    ${PARSED_ARGS_INCLUDES}
  )

  if(PARSED_ARGS_DEFINITIONS)
    target_compile_definitions(${PARSED_ARGS_NAME} PRIVATE ${PARSED_ARGS_DEFINITIONS})
  endif()

  if(NOT CMAKE_CROSSCOMPILING)
    add_test(NAME ${PARSED_ARGS_NAME} COMMAND ${PARSED_ARGS_NAME})
  endif()

  if(INSTALL_UNIT_TESTS)
    install(
      TARGETS ${PARSED_ARGS_NAME} DESTINATION
      ${CMAKE_INSTALL_LIBDIR}/test/${PROJECT_NAME}/${UNIT_NAME}
    )
  endif()
endfunction()
