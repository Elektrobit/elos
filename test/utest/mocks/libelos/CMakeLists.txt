# SPDX-License-Identifier: MIT

# Note: The TARGET_PROPERTY's are used here due to various headaches
#       associated with IMPORTED_LOCATION targets

create_mock_lib(
  NAME
    mock_libelos
  SOURCES
    mock_libelos.c
    mock_libelos_communication.c
  INCLUDES
    $<TARGET_PROPERTY:elos_common_interface,INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:libelos,INCLUDE_DIRECTORIES>
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    libelos_static_weak
    cmocka_extensions::cmocka_extensions
)

target_compile_definitions(mock_libelos PUBLIC MOCK_LIBELOS_FUNC_LINKER_WRAPPING)
target_link_options(
  mock_libelos
  PUBLIC
    -Wl,--wrap=elosConnectTcpip
    -Wl,--wrap=elosDisconnect
    -Wl,--wrap=elosGetVersion
    -Wl,--wrap=elosLogFindEvent
    -Wl,--wrap=elosSessionValid
    -Wl,--wrap=elosEventSubscribe
    -Wl,--wrap=elosEventPublish
    -Wl,--wrap=elosEventQueueRead
    -Wl,--wrap=elosCreateMessage
    -Wl,--wrap=elosSendMessage
    -Wl,--wrap=elosReceiveMessage
    -Wl,--wrap=elosSendJsonMessage
    -Wl,--wrap=elosReceiveJsonMessage
    -Wl,--wrap=elosSendAndReceiveJsonMessage
)

set_target_properties(
  mock_libelos
  PROPERTIES
  SOVERSION ${ELOS_VERSION_MAJOR}
  VERSION ${ELOS_VERSION_MAJOR}.${ELOS_VERSION_MINOR}.${ELOS_VERSION_MICRO}
)

if (ELOS_MOCK_LIBRARY AND INSTALL_ELOS_MOCK_LIBRARY)
  install(TARGETS mock_libelos DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(
    FILES
      mock_libelos.h
    DESTINATION
      ${CMAKE_INSTALL_INCLUDEDIR}/elos/libelos
  )
endif()

