# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.21)

set(RINGBUF_BACKEND_VERSION_MAJOR 0)
set(RINGBUF_BACKEND_VERSION_MINOR 1)
set(RINGBUF_BACKEND_VERSION_MICRO 0)
set(RINGBUF_BACKEND_VERSION
  ${RINGBUF_BACKEND_VERSION_MAJOR}.${RINGBUF_BACKEND_VERSION_MINOR}.${RINGBUF_BACKEND_VERSION_MICRO})

project(
  cpp-ring-buffer-backend
  DESCRIPTION "An in memory ring buffer storage backend plugin for elos written in C++"
  VERSION ${RINGBUF_BACKEND_VERSION}
  LANGUAGES CXX)

set(ELOSD_BACKEND_PATH ${CMAKE_INSTALL_FULL_LIBDIR}/elos/backend)

find_package(safu 0.54.0 REQUIRED)
find_package(samconf 0.53.1 REQUIRED)
find_package(libelosplugin 0.58.4 REQUIRED)

add_library(backend_ring_buffer
  SHARED private/ringBufferBackend.cpp
         private/EventBuffer.cpp)

set_target_properties(backend_ring_buffer PROPERTIES PREFIX "")
set_target_properties(backend_ring_buffer
  PROPERTIES SOVERSION ${RINGBUF_BACKEND_VERSION_MAJOR}
             VERSION ${RINGBUF_BACKEND_VERSION})

target_link_libraries(backend_ring_buffer
    PUBLIC elos::libelosplugin samconf::samconf safu::safu)

target_include_directories(backend_ring_buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS backend_ring_buffer DESTINATION ${ELOSD_BACKEND_PATH})
