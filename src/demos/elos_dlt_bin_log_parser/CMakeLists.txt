# SPDX-License-Identifier: MIT

find_package(safu 0.56.2 REQUIRED)

add_executable(
  elos_dlt_bin_log_parser
  private/elos_dlt_bin_log_parser.c
)

target_include_directories(
  elos_dlt_bin_log_parser
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
  elos_dlt_bin_log_parser
  PUBLIC
    safu::safu
    libelosdlt
)

install(TARGETS elos_dlt_bin_log_parser DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES data/sample_log.dlt DESTINATION ${CMAKE_INSTALL_DATADIR}/elos/elos_dlt_bin_log_parser)
