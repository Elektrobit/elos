# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_interface_library(
  FROM
    scanner_shmem
  LIBRARIES
    libelos_common
    safu::safu_interface
    samconf::samconf_interface
)

add_library(
  scanner_shmem SHARED
  private/shmem_config.c
  private/shmem_ringbuffer.c
  private/shmem.c
)

set_target_properties(scanner_shmem PROPERTIES PREFIX "")

target_link_libraries(
  scanner_shmem
  PUBLIC
    libelos_common
    libelosplugin
    samconf::samconf
    safu::safu
    scanner_shmem_interface
)

target_include_directories(
  scanner_shmem
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS scanner_shmem DESTINATION ${ELOSD_SCANNER_PATH})
endif()

create_static_library_from_shared(
  NAME
    scanner_shmem_static
  FROM
    scanner_shmem
  NO_PREFIX
)

create_weak_library(
  FROM
    scanner_shmem
  TO
    scanner_shmem_static_weak
  WEAKEN_DEPENDENCIES
    scanner_shmem_static
  NO_PREFIX
)
