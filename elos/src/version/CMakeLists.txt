# SPDX-License-Identifier: MIT
project(version)

configure_file(version.c.in src/version/version.c)

add_library(version STATIC "${CMAKE_CURRENT_BINARY_DIR}/src/version/version.c")
target_include_directories(version PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
