# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  punisher-client
  DESCRIPTION "a memory & cpu event handling client plugin for elos"
  LANGUAGES CXX
)
set(CMAKE_INSTALL_FULL_LIBDIR /usr/local/lib)
set(ELOSD_CLIENT_PATH ${CMAKE_INSTALL_FULL_LIBDIR}/elos/client)

find_package(libelosplugin 1.1.1 REQUIRED)
find_package(libelos REQUIRED)
find_package(safu 0.54.0 REQUIRED)
find_package(samconf 0.53.1 REQUIRED)

add_library(
  client_punisher SHARED
  src/punisher.cpp
  src/punisherClient.cpp
)

set_target_properties(client_punisher PROPERTIES PREFIX "")

target_link_libraries(
  client_punisher
  PRIVATE
    elos::libelosplugin
    elos::libelos
    samconf::samconf
    safu::safu
)

target_include_directories(
  client_punisher
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(client_punisher PRIVATE SAFU_LOG=1)
target_compile_options(client_punisher PRIVATE -Wall -Wextra -Wpedantic -Werror)

install(TARGETS client_punisher DESTINATION ${ELOSD_CLIENT_PATH})
