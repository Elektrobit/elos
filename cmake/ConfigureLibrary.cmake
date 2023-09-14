# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.21)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)

function(configureLibrary LIBRARY_NAME LIBRARY_REPOSITORY LIBRARY_TAG)
  message("#### Configuring external library ${LIBRARY_NAME} (${CMAKE_BUILD_TYPE}) ####")

  message("Searching for ${LIBRARY_NAME}")
  find_package(${LIBRARY_NAME} QUIET)
  if(${LIBRARY_NAME}_FOUND)
    message("Found ${LIBRARY_NAME}")
    return()
  endif()
  message("Did not find ${LIBRARY_NAME}")

  SET(LOCAL_REPO_DIR ${LIBRARY_REPOSITORY})


  if (NOT EXISTS ${LIBRARY_REPOSITORY})
    message("Fetching ${LIBRARY_NAME}")

    FetchContent_Declare(
      ${LIBRARY_NAME}
      GIT_REPOSITORY ${LIBRARY_REPOSITORY}
      GIT_TAG ${LIBRARY_TAG}
    )
    FetchContent_MakeAvailable(${LIBRARY_NAME})

    SET(LOCAL_REPO_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/${LIBRARY_NAME}-src/)

    while(NOT EXISTS ${LOCAL_REPO_DIR}/ci/build.sh)
      execute_process(COMMAND ${CMAKE_COMMAND} -E sleep 0.5)
    endwhile()

    message("Done fetching")
  endif()

  message("Building ${LIBRARY_NAME}")

  set(ENV{LOCAL_INSTALL_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../dist)

  execute_process(
  COMMAND ${LOCAL_REPO_DIR}/ci/build.sh ${CMAKE_BUILD_TYPE}
  WORKING_DIRECTORY "${LOCAL_REPO_DIR}"
  COMMAND_ECHO STDOUT
  )

  message("Done building")
  find_package(${LIBRARY_NAME} QUIET)
endfunction()
