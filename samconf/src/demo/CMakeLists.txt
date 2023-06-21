# SPDX-License-Identifier: MIT
add_executable(
  samprobe
  samprobe.c
)

target_link_libraries(
  samprobe
  PRIVATE
    samconf
)

install(TARGETS samprobe DESTINATION ${CMAKE_INSTALL_BINDIR})
