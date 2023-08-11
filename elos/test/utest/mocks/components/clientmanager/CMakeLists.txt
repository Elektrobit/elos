# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_clientmanager
  SOURCES
    mock_clientmanager.c
    mock_clientauthorization.c
    mock_clientauthorizedprocesses.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    clientmanager_static_weak
    cmocka_extensions::cmocka_extensions
)
target_compile_definitions(mock_clientmanager PUBLIC MOCK_ELOS_CLIENT_MANAGER_FUNC_LINKER_WRAPPING)
target_link_options(
  mock_clientmanager
  PUBLIC
    -Wl,--wrap=elosClientManagerGetStatus
    -Wl,--wrap=elosClientManagerThreadGetFreeConnectionSlot
    -Wl,--wrap=elosClientManagerThreadWaitForIncomingConnection
    -Wl,--wrap=elosClientAuthorizationInitialize
    -Wl,--wrap=elosClientAuthorizationDelete
    -Wl,--wrap=elosClientAuthorizationIsTrustedConnection
    -Wl,--wrap=elosAuthorizedProcessInitialize
    -Wl,--wrap=elosAuthorizedProcessDelete
    -Wl,--wrap=elosAuthorizedProcessCheck
)
