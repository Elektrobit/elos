# SPDX-License-Identifier: MIT
find_package(samconf REQUIRED)

add_compile_definitions(ELOSD_LOG_PREFIX="${ELOSD_LOG_PREFIX}")

add_subdirectory(dummy)
add_subdirectory(jsonbackend)
