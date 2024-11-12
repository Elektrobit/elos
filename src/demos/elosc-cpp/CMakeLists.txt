# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

enable_language(CXX)

add_executable(elosc-cpp elosc-cpp.cpp)

target_link_libraries(elosc-cpp PRIVATE libelos-cpp_static safu::safu)

add_executable(elosc-publish-cpp elosc-publish-cpp.cpp)

target_link_libraries(elosc-publish-cpp PRIVATE libelos-cpp_static safu::safu)

if(INSTALL_ELOS_DEMOS)
  install(TARGETS elosc-cpp DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(TARGETS elosc-publish-cpp DESTINATION ${CMAKE_INSTALL_BINDIR})
endif(INSTALL_ELOS_DEMOS)
