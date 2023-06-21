# SPDX-License-Identifier: MIT

add_library(scanner_interface INTERFACE)

target_include_directories(
  scanner_interface
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
