# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.21)

include(cmake/project.cmake)

project(
  elos
    DESCRIPTION "Event LOgging and managment System"
    VERSION ${ELOS_VERSION}
    LANGUAGES C
)


option(ELOS_BUILD_DEFAULTS "enable all default builds" ON)

option(ENABLE_ANALYZER "Build with -fanalyzer" ON)
option(ENABLE_ASAN "Link with ASAN" ON)
option(ENABLE_CI "Use CI mode for building" OFF)
option(ENABLE_GIT_VERSION "Enable the git hash for the version" ON)

option(ELOS_DAEMON "Build elosd" ${ELOS_BUILD_DEFAULTS})
option(INSTALL_ELOS_DAEMON "Install elosd" ON)

option(ELOS_COMMON "build libelos_common" ${ELOS_BUILD_DEFAULTS})

option(ELOS_LIBRARY "build libelos" ${ELOS_BUILD_DEFAULTS})
option(ELOS_LIBRARY_CPP "build libelos-cpp" ${ELOS_BUILD_DEFAULTS})

option(ELOS_PLUGIN_LIBRARY "build libelosplugin" ${ELOS_BUILD_DEFAULTS})

option(ELOS_LIBRARY_LITE "build libelos-lite" OFF)

option(ELOS_TOOLS "Build elosc and other tools" ${ELOS_BUILD_DEFAULTS})
option(INSTALL_ELOS_TOOLS "Install elosc and other tools" ON)

option(ELOS_DEMOS "Build the elos demos" ${ELOS_BUILD_DEFAULTS})
option(INSTALL_ELOS_DEMOS "Install the elos demos" ON)

option(UNIT_TESTS "Build unit tests" ${ELOS_BUILD_DEFAULTS})
option(INSTALL_UNIT_TESTS "Install unit tests" ON)

option(SMOKE_TESTS "Build smoke tests" ${ELOS_BUILD_DEFAULTS})
option(INSTALL_SMOKE_TESTS "Build smoke tests" OFF)

option(ELOS_MOCK_LIBRARY "Build the mock libraries" OFF)
option(INSTALL_ELOS_MOCK_LIBRARY "Install the mock libraries" ON)

option(ELOS_PLUGINS "Build the elos plugins" ${ELOS_BUILD_DEFAULTS})
option(INSTALL_ELOS_PLUGINS "install the elos plugins" ON)

option(ELOSD_SYSTEMD "Enable systemd features (startup notification and socket-based activation for the syslog scanner)" OFF)

option(ELOSD_EVENTLOGGING_BACKEND_SQL "Build sqlite3 bakend plugin" ${ELOS_BUILD_DEFAULTS})
option(ELOSD_EVENTLOGGING_BACKEND_INFLUXDB "Build InfluxDB backend plugin" OFF)
option(ELOSD_EVENTLOGGING_BACKEND_DLT "Build DLT backend plugin" ${ELOS_BUILD_DEFAULTS})
option(ELOSD_LIBDLT "Build libelosdlt, a dlt protocol implementation." ${ELOSD_EVENTLOGGING_BACKEND_DLT})

option(ENABLE_LOGGING "Enables the safuLog functions" OFF)

project_set_environment()
project_set_version_variables()
project_add_documentation_target(
  TITLE
    "BaseOS ElosD Documentation"
  MARKDOWN
    ${CMAKE_SOURCE_DIR}/documentation/documentation.md
)

if(ENABLE_LOGGING)
  add_compile_definitions(SAFU_LOG=1)
endif(ENABLE_LOGGING)

add_subdirectory(src)
if(UNIT_TESTS)
  enable_testing()
  add_subdirectory(test/tsan)
endif(UNIT_TESTS)
add_subdirectory(test/utest)
add_subdirectory(test/smoketest)

if (ELOS_DAEMON)
  option(INSTALL_ELOSD_SYSTEMD_UNIT "install the elosd.service unit" ${ELOSD_SYSTEMD})
  option(INSTALL_ELOSD_SYSVINIT_SCRIPT "install the elosd init script" OFF)
  set(INSTALL_ELOSD_SYSTEMD_UNIT_DIR "${CMAKE_INSTALL_LIBDIR}/systemd/system" CACHE STRING "destination directory for the service unit")
  set(INSTALL_ELOSD_SYSVINIT_SCRIPT_DIR "${CMAKE_INSTALL_SYSCONFDIR}/init.d" CACHE STRING "destination directory for the elosd init script")
  add_subdirectory(integration)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
