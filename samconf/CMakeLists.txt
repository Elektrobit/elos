# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.21)
include(project.cmake)

project(samconf LANGUAGES C VERSION ${ELOS_VERSION})

project_set_environment()
project_set_version_variables()
project_add_documentation_target(
  TITLE
    "BaseOS SamConf Documentation"
  MARKDOWN
    ${CMAKE_SOURCE_DIR}/documentation/documentation.md
)

option(JSON_BACKEND "Enable Json Backend Support" ON)

if(JSON_BACKEND)
	add_compile_options(-DSAMCONF_ENABLE_CONFIG_BACKEND_JSON)
endif(JSON_BACKEND)

add_subdirectory(src)
if(UNIT_TESTS)
  enable_testing()
  add_subdirectory(test/utest)
endif(UNIT_TESTS)

if(PACKAGING)
  include(Packing.cmake)
endif(PACKAGING)
