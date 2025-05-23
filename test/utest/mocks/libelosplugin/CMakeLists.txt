# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_libelosplugin
  SOURCES
    mock_plugincontrol.c
    mock_connectionmanager.c
    mock_clientauthorizedprocesses.c
    mock_message_handler.c
    private/plugincontrol_stubs.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    libelosplugin_static_weak
    cmocka_extensions::cmocka_extensions
    json-c
    WRAP_FLAG
    MOCK_ELOS_LIBELOSPLUGIN_FUNC_LINKER_WRAPPING
  WRAPS
    elosPluginControlInitialize
    elosPluginControlNew
    elosPluginControlLoad
    elosPluginControlUnload
    elosPluginControlStart
    elosPluginControlStop
    elosPluginControlDeleteMembers
    elosPluginControlDelete
    elosPluginControlGetName
    elosPluginControlPtrVectorInitialize
    elosPluginControlPtrVectorPush
    elosPluginControlPtrVectorIterate
    elosPluginControlPtrVectorGetById
    elosPluginControlPtrVectorRemoveById
    elosPluginControlPtrVectorDeleteMembers
    elosPluginControlCreatePublisher
    elosPluginControlDeletePublisher
    elosConnectionManagerThreadGetFreeConnectionSlot
    elosConnectionManagerThreadWaitForIncomingConnection
    elosAuthorizedProcessInitialize
    elosAuthorizedProcessDelete
    elosAuthorizedProcessCheck
    elosMessageHandlerSendJson
    elosMessageEventCreate
    elosMessageEventPush
    elosMessageEventRemove
    elosMessageEventlistCreate
    elosMessageEventlistGetLimit
    elosMessageEventlistRead
    elosMessageEventlistRemove
    elosMessageEventlistSetLimit
    elosMessageGetVersion
    elosMessageHandlerResponseCreate
)
