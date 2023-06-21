# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.21)
include(project.cmake)

project(elos LANGUAGES C VERSION ${ELOS_VERSION})

project_set_environment()
project_set_version_variables()
project_add_documentation_target(
  TITLE
    "BaseOS ElosD Documentation"
  MARKDOWN
    ${CMAKE_SOURCE_DIR}/documentation/documentation.md
)

add_subdirectory(src)
if(UNIT_TESTS)
  enable_testing()
  add_subdirectory(test/utest)
endif(UNIT_TESTS)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(PACKAGING)
  include(Packing.cmake)
endif(PACKAGING)

