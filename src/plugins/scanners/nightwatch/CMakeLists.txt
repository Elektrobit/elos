# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  nightwatch-scanner
  DESCRIPTION "a memory & cpu monitoring scanner plugin for elos"
  LANGUAGES CXX
)
set(CMAKE_INSTALL_FULL_LIBDIR /usr/local/lib)
set(ELOSD_SCANNER_PATH ${CMAKE_INSTALL_FULL_LIBDIR}/elos/scanner)

find_package(libelosplugin 1.1.1 REQUIRED)
find_package(libelos REQUIRED)
find_package(safu 0.54.0 REQUIRED)
find_package(samconf 0.53.1 REQUIRED)
find_package(YAML-CPP REQUIRED)

add_library(
  scanner_nightwatch SHARED
  src/nightwatchScanner.cpp
  src/systemMonitoring.cpp
  src/monitoringUnit.cpp
  src/application.cpp
)

set_target_properties(scanner_nightwatch PROPERTIES PREFIX "")

target_link_libraries(
  scanner_nightwatch
  PRIVATE
    elos::libelosplugin
    elos::libelos
    samconf::samconf
    safu::safu
    yaml-cpp
)

target_include_directories(
  scanner_nightwatch
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(scanner_nightwatch PRIVATE SAFU_LOG=1)
target_compile_options(scanner_nightwatch PRIVATE -Wall -Wextra -Wpedantic -Werror)

install(TARGETS scanner_nightwatch DESTINATION ${ELOSD_SCANNER_PATH})
install(FILES ./config/config.yaml DESTINATION /etc/elos/nightwatch)
