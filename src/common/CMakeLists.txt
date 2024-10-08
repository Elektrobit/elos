# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)

include(../../cmake/shared_library.cmake)

add_library(elos_common_interface INTERFACE)

set_property(TARGET elos_common_interface PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(
  elos_common_interface
  INTERFACE
  ELOSD_HARDWAREID_FILE="${ELOSD_HARDWAREID_FILE}"
  ELOSD_HARDWAREID_MAX_LENGTH=${ELOSD_HARDWAREID_MAX_LENGTH}
  ELOSD_HARDWAREID_DEFAULT_VALUE="${ELOSD_HARDWAREID_DEFAULT_VALUE}"
  ELOSD_HARDWAREID_ENVIRONMENT_PREFIX="${ELOSD_HARDWAREID_ENVIRONMENT_PREFIX}"
  ELOSD_SCANNER_PATH="${ELOSD_SCANNER_PATH}"
  ELOSD_BACKEND_PATH="${ELOSD_BACKEND_PATH}"
  ELOSD_LOG_FILTER="${ELOSD_LOG_FILTER}"
  ELOSD_LOG_LEVEL="${ELOSD_LOG_LEVEL}"
)

target_include_directories(
  elos_common_interface
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interface/>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
  elos_common_interface
  INTERFACE
    safu::safu
)
