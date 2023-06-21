# SPDX-License-Identifier: MIT
add_executable(
  elos-coredump
  coredump.c
)

target_link_libraries(
  elos-coredump
  PRIVATE
    libelos
    version
)

install(TARGETS elos-coredump DESTINATION ${CMAKE_INSTALL_BINDIR})
