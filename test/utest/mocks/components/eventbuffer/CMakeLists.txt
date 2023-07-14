# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_eventbuffer
  SOURCES
    mock_eventbuffer.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    eventbuffer_static_weak
    cmocka_extensions::cmocka_extensions
  NO_DYNAMIC_LIBRARY
)
