# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_rpnfilter
  SOURCES
    mock_rpnfilter.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    rpnfilter_static_weak
    cmocka_extensions::cmocka_extensions
  WRAP_FLAG
    MOCK_RPNFILTER_FUNC_LINKER_WRAPPING
  WRAPS
    elosRpnFilterDeleteMembers
    elosRpnFilterVectorPush
)
