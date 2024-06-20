# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(samconf 0.50.1 REQUIRED)

create_interface_library(
  FROM
    plugincontrol
  LIBRARIES
    libelosplugin_interface
    elos_common_interface
    eventlogging_interface
    samconf::samconf_interface
    safu::safu_interface
)

add_library(
  plugincontrol_static
  STATIC
    ./private/plugincontrol_load.c
    ./private/plugincontrol_vector.c
    ./private/plugincontrol_workerthread.c
    ./private/plugincontrol.c
)

target_include_directories(
  plugincontrol_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  plugincontrol_static
  PUBLIC
    plugincontrol_interface
    eventprocessor_static
    eventdispatcher_static
    eventbuffer_static
    eventlogging_static
    samconf::samconf
    safu::safu
)

set_property(TARGET plugincontrol_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    plugincontrol_static
)
