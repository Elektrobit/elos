# SPDX-License-Identifier: MIT
find_package(cmocka_extensions REQUIRED)
find_package(json-c REQUIRED)

add_library(
  mock_jsonc SHARED
  mock_jsonc.c
)

set_property(TARGET mock_jsonc PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(
  mock_jsonc
  PROPERTIES
    SOVERSION ${CMOCKA_MOCKS_VERSION_MAJOR}
    VERSION ${CMOCKA_MOCKS_VERSION_MAJOR}.${CMOCKA_MOCKS_VERSION_MINOR}.${CMOCKA_MOCKS_VERSION_MICRO}
)

target_include_directories(
  mock_jsonc
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PUBLIC
    $<INSTALL_INTERFACE:include/>
)

target_link_options(
 mock_jsonc
 PUBLIC
   -Wl,--wrap=json_object_new_object
   -Wl,--wrap=json_object_get
   -Wl,--wrap=json_object_put
   -Wl,--wrap=json_object_is_type
   -Wl,--wrap=json_object_get_type
   -Wl,--wrap=json_object_to_json_string
   -Wl,--wrap=json_object_to_json_string_ext
   -Wl,--wrap=json_object_to_json_string_length
   -Wl,--wrap=json_object_get_userdata
   -Wl,--wrap=json_object_set_userdata
   -Wl,--wrap=json_object_set_serializer
   -Wl,--wrap=json_object_get_object
   -Wl,--wrap=json_object_object_length
   -Wl,--wrap=json_object_object_add
   -Wl,--wrap=json_object_object_add_ex
   -Wl,--wrap=json_object_object_get
   -Wl,--wrap=json_object_object_get_ex
   -Wl,--wrap=json_object_object_del
   -Wl,--wrap=json_object_new_array
   -Wl,--wrap=json_object_new_array_ext
   -Wl,--wrap=json_object_get_array
   -Wl,--wrap=json_object_array_length
   -Wl,--wrap=json_object_array_add
   -Wl,--wrap=json_object_array_put_idx
   -Wl,--wrap=json_object_array_get_idx
   -Wl,--wrap=json_object_array_del_idx
   -Wl,--wrap=json_object_array_shrink
   -Wl,--wrap=json_object_new_boolean
   -Wl,--wrap=json_object_get_boolean
   -Wl,--wrap=json_object_set_boolean
   -Wl,--wrap=json_object_new_int
   -Wl,--wrap=json_object_new_int64
   -Wl,--wrap=json_object_new_uint64
   -Wl,--wrap=json_object_get_int
   -Wl,--wrap=json_object_set_int
   -Wl,--wrap=json_object_int_inc
   -Wl,--wrap=json_object_get_int64
   -Wl,--wrap=json_object_get_uint64
   -Wl,--wrap=json_object_set_int64
   -Wl,--wrap=json_object_set_uint64
   -Wl,--wrap=json_object_new_double
   -Wl,--wrap=json_object_new_double_s
   -Wl,--wrap=json_c_set_serialization_double_format
   -Wl,--wrap=json_object_double_to_json_string
   -Wl,--wrap=json_object_get_double
   -Wl,--wrap=json_object_set_double
   -Wl,--wrap=json_object_new_string
   -Wl,--wrap=json_object_new_string_len
   -Wl,--wrap=json_object_get_string
   -Wl,--wrap=json_object_get_string_len
   -Wl,--wrap=json_object_set_string
   -Wl,--wrap=json_object_set_string_len
   -Wl,--wrap=json_object_new_null
   -Wl,--wrap=json_object_equal
   -Wl,--wrap=json_object_deep_copy
   -Wl,--wrap=json_tokener_parse
   -Wl,--wrap=json_object_from_file
   -Wl,--wrap=json_util_get_last_err
)

target_link_libraries(
  mock_jsonc
  PRIVATE
    cmocka
    cmocka_extensions::cmocka_extensions
    json-c::json-c
)

install(
  EXPORT mock_jsoncTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks
  NAMESPACE cmocka_mocks::
  FILE mock_jsoncTargets.cmake # Not sure if this is still needed
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/mock_jsoncConfigVersion.cmake
  VERSION ${PKG_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mock_jsoncConfigVersion.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/cmocka_mocks"
)

install(TARGETS mock_jsonc EXPORT mock_jsoncTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
