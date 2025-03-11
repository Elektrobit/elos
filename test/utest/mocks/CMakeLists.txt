# SPDX-License-Identifier: MIT
include(create_mock_lib.cmake)
find_package(cmocka_mocks 0.54.2 REQUIRED)

add_subdirectory(components)
add_subdirectory(elosd)
add_subdirectory(libelos)
add_subdirectory(libelosplugin)
add_subdirectory(plugins)
