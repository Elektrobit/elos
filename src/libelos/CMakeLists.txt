# SPDX-License-Identifier: MIT

find_package(json-c REQUIRED)
find_package(safu REQUIRED)

# Installs interface and public headers from the given components.
# Needed for libelos.h to work properly after installation.
function(install_component_headers)
  cmake_parse_arguments(PARAM "" "" "COMPONENTS" ${ARGN})
  foreach(COMPONENT IN LISTS PARAM_COMPONENTS)
    get_target_property(COMPONENT_INCLUDE_DIRS ${COMPONENT} INTERFACE_INCLUDE_DIRECTORIES)
    foreach(COMPONENT_INCLUDE_DIR IN LISTS COMPONENT_INCLUDE_DIRS)
      list(APPEND INCLUDE_DIRS ${COMPONENT_INCLUDE_DIR})
    endforeach()
  endforeach()
  list(REMOVE_DUPLICATES INCLUDE_DIRS)
  foreach(INCLUDE_DIR IN LISTS INCLUDE_DIRS)
    install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endforeach()
endfunction()

add_library(
  libelos
  SHARED
    private/libelos.c
    private/libelos_communication.c
    private/libelos_logger.c
)

target_include_directories(
  libelos
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_link_libraries(
  libelos
  PUBLIC
    elos_common_interface
    elos_event_static
    eloslog_static
    json-c::json-c
    safu::safu
)

set_target_properties(libelos PROPERTIES PREFIX "")  # Prevent creation of "liblibelos.so"
set_target_properties(
  libelos
  PROPERTIES
    SOVERSION ${ELOS_VERSION_MAJOR}
    VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

install(TARGETS libelos DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install_component_headers(COMPONENTS elos_common_interface elos_event_static)

create_static_library_from_shared(
  NAME
    libelos_static
  FROM
    libelos
  NO_PREFIX
)

create_weak_library(
  FROM
    libelos
  TO
    libelos_static_weak
  WEAKEN_DEPENDENCIES
    elos_event_static
  NO_PREFIX
)
