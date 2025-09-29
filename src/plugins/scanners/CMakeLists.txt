# SPDX-License-Identifier: MIT
find_package(samconf 0.56.3 REQUIRED)

add_compile_definitions(ELOSD_LOG_PREFIX="${ELOSD_LOG_PREFIX}")

add_subdirectory(dummy)
add_subdirectory(kmsg)
add_subdirectory(shmem)
add_subdirectory(syslog)
add_subdirectory(oomkiller)
if (ELOSD_LIBDLT)
  add_subdirectory(dlt_hv)
endif (ELOSD_LIBDLT)
