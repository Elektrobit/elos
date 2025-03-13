# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_unix_config
  SOURCES
    mock_unix_config.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    unix_config_static_weak
    cmocka_extensions::cmocka_extensions
  WRAP_FLAG
    MOCK_UNIX_CONFIG_FUNC_LINKER_WRAPPING
  WRAPS
    elosUnixConfigGetPath
    elosUnixConfigGetSocketAddress
    elosUnixConfigGetConnectionLimit
)
