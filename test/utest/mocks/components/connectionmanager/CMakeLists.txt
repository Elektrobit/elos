# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_connectionmanager
  SOURCES
    mock_connectionmanager.c
    mock_clientauthorization.c
    mock_clientauthorizedprocesses.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    connectionmanager_static_weak
    cmocka_extensions::cmocka_extensions
)
target_compile_definitions(mock_connectionmanager PUBLIC MOCK_ELOS_CONNECTION_MANAGER_FUNC_LINKER_WRAPPING)
target_link_options(
  mock_connectionmanager
  PUBLIC
    -Wl,--wrap=elosConnectionManagerGetStatus
    -Wl,--wrap=elosConnectionManagerThreadGetFreeConnectionSlot
    -Wl,--wrap=elosConnectionManagerThreadWaitForIncomingConnection
    -Wl,--wrap=elosClientAuthorizationInitialize
    -Wl,--wrap=elosClientAuthorizationDelete
    -Wl,--wrap=elosClientAuthorizationIsTrustedConnection
    -Wl,--wrap=elosAuthorizedProcessInitialize
    -Wl,--wrap=elosAuthorizedProcessDelete
    -Wl,--wrap=elosAuthorizedProcessCheck
)
