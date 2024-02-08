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

if (INSTALL_ELOS_TOOLS)
  install(TARGETS elos-coredump DESTINATION ${CMAKE_INSTALL_BINDIR})
endif (INSTALL_ELOS_TOOLS)
