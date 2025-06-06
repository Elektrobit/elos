# SPDX-License-Identifier: MIT
find_package(samconf 0.56.3 REQUIRED)
find_package(safu 0.56.2 REQUIRED)

add_executable(
  elos-coredump
  coredump.c
)

set(ELOS_COREDUMP_CONFIG_FILE "${CMAKE_INSTALL_FULL_SYSCONFDIR}/elos/coredump.json")
add_compile_definitions(ELOS_COREDUMP_CONFIG_FILE="${ELOS_COREDUMP_CONFIG_FILE}")
add_compile_definitions(SAFU_LOG=1)

target_link_libraries(
  elos-coredump
  PRIVATE
    libelos
    version
    samconf::samconf
    safu::safu
)

if (INSTALL_ELOS_TOOLS)
  install(TARGETS elos-coredump DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(FILES coredump.json DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/elos)
endif (INSTALL_ELOS_TOOLS)
