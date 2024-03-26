# SPDX-License-Identifier: MIT
find_package(samconf 0.50.1 REQUIRED)

add_compile_definitions(ELOSD_LOG_PREFIX="${ELOSD_LOG_PREFIX}")

add_subdirectory(dummy)
add_subdirectory(jsonbackend)
if(ELOSD_EVENTLOGGING_BACKEND_INFLUXDB)
  add_subdirectory(influxdbbackend)
endif()
add_subdirectory(sqlbackend)
if(ELOSD_EVENTLOGGING_BACKEND_NOSQL)
  add_subdirectory(nosqlbackend)
endif()
if(ELOSD_EVENTLOGGING_BACKEND_DLT)
  add_subdirectory(dlt)
endif()
