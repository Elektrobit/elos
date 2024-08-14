# SPDX-License-Identifier: MIT
find_package(safu 0.54.0 REQUIRED)
find_package(samconf 0.53.1 REQUIRED)

enable_language(CXX)

add_library(backend_fetchapi
  SHARED private/fetchApiBackend.cpp
         private/EventBuffer.cpp)

create_interface_library(
  FROM
    backend_fetchapi
  LIBRARIES
    safu::safu_interface
    samconf::samconf_interface
    libelosplugin
)

set_target_properties(backend_fetchapi PROPERTIES PREFIX "")

target_link_libraries(backend_fetchapi
    PUBLIC libelosplugin samconf::samconf safu::safu)

target_include_directories(backend_fetchapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (INSTALL_ELOS_PLUGINS)
  install(TARGETS backend_fetchapi DESTINATION ${ELOSD_BACKEND_PATH})
endif()

create_static_library_from_shared(
  NAME
    backend_fetchapi_static
  FROM
    backend_fetchapi
  NO_PREFIX
)
