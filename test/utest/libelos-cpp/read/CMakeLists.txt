# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

enable_language(CXX)

create_unit_test(
  NAME
    test_libelos-cpp_read_utest
  SOURCES
    case_success.cpp
    case_error.cpp
    read_utest.cpp
  LIBRARIES
    libelos-cpp_static
    mock_libelos_static
)
