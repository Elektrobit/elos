# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_eventdispatcher
  SOURCES
    mock_eventdispatcher.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    eventdispatcher_static_weak
    cmocka_extensions::cmocka_extensions
  NO_DYNAMIC_LIBRARY
)
