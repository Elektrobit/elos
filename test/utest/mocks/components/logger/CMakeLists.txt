# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_logger
  SOURCES
    mock_logger.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    logger_static_weak
    cmocka_extensions::cmocka_extensions
  NO_DYNAMIC_LIBRARY
)
