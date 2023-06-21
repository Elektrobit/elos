# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_client_manager
  SOURCES
    mock_client_manager.c
    mock_client_authorization.c
    mock_client_authorized_processes.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    clientmanager_static_weak
    cmocka_extensions::cmocka_extensions
)
target_compile_definitions(mock_client_manager PUBLIC MOCK_ELOS_CLIENT_MANAGER_FUNC_LINKER_WRAPPING)
target_link_options(
  mock_client_manager
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
