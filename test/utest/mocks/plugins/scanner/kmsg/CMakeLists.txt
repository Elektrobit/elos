# SPDX-License-Identifier: MIT
find_package(samconf 0.56.3 REQUIRED)
find_package(cmocka_mocks 0.55.3 REQUIRED)

create_mock_lib(
  NAME
    mock_scanner_kmsg
  SOURCES
    mock_kmsg_mapper.c
  INCLUDES
    $<TARGET_PROPERTY:scanner_kmsg,INCLUDE_DIRECTORIES>
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES_STATIC
    scanner_kmsg_static_weak
  LIBRARIES
    cmocka_extensions::cmocka_extensions
  LIBRARIES_DYNAMIC
    scanner_kmsg
  WRAP_FLAG
    MOCK_KMSG_FUNC_LINKER_WRAPPING
  WRAPS
    elosKmsgMapperInit
)
