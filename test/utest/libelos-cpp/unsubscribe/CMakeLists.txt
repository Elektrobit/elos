# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

enable_language(CXX)

create_unit_test(
  NAME
    test_libelos-cpp_unsubscribe_utest
  SOURCES
    case_success.cpp
    unsubscribe_utest.cpp
  LIBRARIES
    mock_libelos_static
    libelos-cpp_static
)
