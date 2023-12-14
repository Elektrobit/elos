# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_clientmanager
  SOURCES
    mock_clientmanager.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    clientmanager_static_weak
    cmocka_extensions::cmocka_extensions
  WRAP_FLAG
    MOCK_ELOS_CLIENT_MANAGER_FUNC_LINKER_WRAPPING
  WRAPS
    elosClientManagerInitialize
    elosClientManagerStart
    elosClientManagerStop
    elosClientManagerDeleteMembers
)
