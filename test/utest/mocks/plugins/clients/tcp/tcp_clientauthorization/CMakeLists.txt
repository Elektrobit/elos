# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_tcp_clientauthorization
  SOURCES
    mock_tcp_clientauthorization.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    tcp_clientauthorization_static_weak
    cmocka_extensions::cmocka_extensions
  WRAP_FLAG
    MOCK_TCP_CLIENTAUTHORIZATION_FUNC_LINKER_WRAPPING
  WRAPS
    elosTcpClientAuthorizationInitialize
    elosTcpClientAuthorizationDelete
    elosTcpClientAuthorizationIsTrustedConnection
)
