# SPDX-License-Identifier: MIT

add_subdirectory(syslog)
add_subdirectory(kmsg)
if (ELOSD_LIBDLT)
  add_subdirectory(dlt_hv)
endif (ELOSD_LIBDLT)
