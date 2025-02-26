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
    $<TARGET_PROPERTY:libelosplugin_static,INCLUDE_DIRECTORIES>
  LIBRARIES
    client_tcp
    libelosplugin_static_weak
    cmocka_extensions::cmocka_extensions
)
target_compile_definitions(mock_connectionmanager PUBLIC MOCK_ELOS_CONNECTION_MANAGER_FUNC_LINKER_WRAPPING)
target_link_options(
  mock_connectionmanager
  PUBLIC
    -Wl,--wrap=elosConnectionManagerThreadGetFreeConnectionSlot
    -Wl,--wrap=elosConnectionManagerThreadWaitForIncomingConnection
    -Wl,--wrap=elosTcpClientAuthorizationInitialize
    -Wl,--wrap=elosTcpClientAuthorizationDelete
    -Wl,--wrap=elosTcpClientAuthorizationIsTrustedConnection
    -Wl,--wrap=elosAuthorizedProcessInitialize
    -Wl,--wrap=elosAuthorizedProcessDelete
    -Wl,--wrap=elosAuthorizedProcessCheck
)
