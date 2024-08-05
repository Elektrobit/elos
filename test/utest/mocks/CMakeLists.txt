# SPDX-License-Identifier: MIT
include(create_mock_lib.cmake)
find_package(cmocka_mocks REQUIRED)

add_subdirectory(components)
add_subdirectory(libelos)
