# SPDX-License-Identifier: MIT
find_package(samconf 0.56.3 REQUIRED)

add_compile_definitions(ELOSD_LOG_PREFIX="${ELOSD_LOG_PREFIX}")

add_subdirectory(fetchapi)
add_subdirectory(dummy)
add_subdirectory(jsonbackend)
if(ELOSD_EVENTLOGGING_BACKEND_INFLUXDB)
  add_subdirectory(influxdbbackend)
endif()
if(ELOSD_EVENTLOGGING_BACKEND_SQL)
add_subdirectory(sqlbackend)
endif()
if(ELOSD_EVENTLOGGING_BACKEND_DLT)
  add_subdirectory(dlt)
endif()
