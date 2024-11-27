# SPDX-License-Identifier: MIT

if(SMOKE_TESTS)
  if(INSTALL_SMOKE_TESTS)
    install(
      DIRECTORY
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/test/smoketest>
      DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/test/${PROJECT_NAME}/
      PATTERN "CMakeLists.txt" EXCLUDE
      PATTERN "smoketest_interactive.sh" EXCLUDE
      PATTERN "index.rst" EXCLUDE
      PATTERN "*.sh" PERMISSIONS
          OWNER_READ OWNER_EXECUTE OWNER_WRITE
          GROUP_READ GROUP_EXECUTE GROUP_WRITE
          WORLD_READ WORLD_EXECUTE
    )
  endif(INSTALL_SMOKE_TESTS)
endif(SMOKE_TESTS)
