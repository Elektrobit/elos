# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

add_compile_definitions(SAFU_LOG="LOG")

create_interface_library(
  FROM
    storagemanager
  LIBRARIES
    elos_event_interface
    storagemanager_interface
    pluginmanager_interface
    samconf::samconf_interface
)


add_library(
  storagemanager_static
  STATIC
    ./private/PluginFilterLoader.c
    ./private/StorageBackend.c
    ./private/storagebackend_vector.c
    ./private/storagemanager.c
)

target_include_directories(
  storagemanager_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  storagemanager_static
  PUBLIC
    storagemanager_interface
    eventfilter_static
    elos_common_interface
    pluginmanager_static
    config_static
    safu::safu
    samconf::samconf
)

set_property(TARGET storagemanager_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    storagemanager_static
  WEAKEN_DEPENDENCIES
    pluginmanager_static
    config_static
)
