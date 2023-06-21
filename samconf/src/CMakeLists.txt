# SPDX-License-Identifier: MIT
include(weak_library.cmake)
include(static_from_shared.cmake)

add_subdirectory(demo)
add_subdirectory(samconf)

install(FILES ${PROJECT_SOURCE_DIR}/src/signature.sh
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
	DESTINATION ${CMAKE_INSTALL_BINDIR})
