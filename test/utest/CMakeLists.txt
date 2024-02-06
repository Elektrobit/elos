# SPDX-License-Identifier: MIT
include(unit_test.cmake)

if(UNIT_TESTS)
  add_subdirectory(components)
  add_subdirectory(libelos)
  add_subdirectory(plugins)
  add_subdirectory(version)
endif(UNIT_TESTS)
if (ELOS_MOCK_LIBRARY OR UNIT_TESTS)
  add_subdirectory(mocks)
endif()
