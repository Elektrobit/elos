# SPDX-License-Identifier: MIT
find_package(safu)
find_package(samconf)

create_interface_library(
  FROM
    eventprocessor
  LIBRARIES
    eventfilter_interface
    safu::safu_interface
    samconf::samconf_interface
)

add_library(
  eventprocessor_static
  STATIC
    ./private/eventfiltermanager/eventfiltermanager.c
    ./private/eventfiltermanager/eventfilternode.c
    ./private/eventfiltermanager/eventfilternodevector.c
    ./private/eventprocessor/eventprocessor.c
    ./private/eventqueue/eventqueue.c
    ./private/eventqueue/eventqueuevector.c
    ./private/eventqueue/eventqueuemanager.c
)

target_include_directories(
  eventprocessor_static
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(
  eventprocessor_static
  PUBLIC
    eventprocessor_interface
    elos_common_interface
    elos_event_static
    safu::safu
    eventfilter_static
)

set_property(TARGET eventprocessor_static PROPERTY POSITION_INDEPENDENT_CODE ON)

create_weak_library(
  FROM
    eventprocessor_static
  WEAKEN_DEPENDENCIES
    elos_event_static
    eventfilter_static
)
