# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_eventfilter
  SOURCES
    mock_eventfilter.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    eventfilter_static_weak
    cmocka_extensions::cmocka_extensions
  WRAP_FLAG
    MOCK_EVENTFILTER_FUNC_LINKER_WRAPPING
  WRAPS
    elosEventFilterNew
    elosEventFilterInit
    elosEventFilterDeleteMembers
    elosEventFilterDelete
    elosEventFilterDeepCopy
    elosEventFilterClone
    elosEventFilterCreate
    elosEventFilterExecute
)
